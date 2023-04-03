#pragma once

#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>

#include "CacheBackend.h"

class CacheMemory : CacheBackend {

public:

    CacheMemory() {}
    ~CacheMemory() {}

    void add(Order order) {
        m_orders.insert(order);
    }

    void remove(const std::string& orderId) {
        auto it = std::find_if(m_orders.begin(), m_orders.end(),
            [&](Order order) { return order.orderId() == orderId; });

        if (it != m_orders.end()) {
            m_orders.erase(it);
        }
    }

    Order get(const std::string& orderId) {
        auto it = std::find_if(m_orders.begin(), m_orders.end(),
            [&](Order order) { return order.orderId() == orderId; });

        return *it;
    }

    void update(Order order) {
        remove(order.orderId());
        add(order);
    }

    std::vector<Order> getOrders() const {
        std::vector<Order> orders;

        orders.insert(orders.end(), m_orders.begin(), m_orders.end());

        return orders;
    }

    std::vector<Order> getOrdersBySecurity(const std::string& securityId) {
        std::vector<Order> orders;

        for (auto order : m_orders) {
            if (order.securityId() == securityId) {
                orders.push_back(order);
            }
        }

        return orders;
    }

    std::vector<Order> getOrdersByCompany(const std::string& company) {
        std::vector<Order> orders;

        for (auto order : m_orders) {
            if (order.company() == company) {
                orders.push_back(order);
            }
        }

        return orders;
    }

    std::vector<Order> getOrdersByUser(const std::string& user) {
        std::vector<Order> orders;

        for (auto order : m_orders) {
            if (order.user() == user) {
                orders.push_back(order);
            }
        }

        return orders;
    }

    std::vector<Order> getMatches(Order& order) {
        //we could remove this comparator and overload Order < operator
        const auto cmp = [](const Order& a, const Order& b) {
            return a.orderId() < b.orderId();
        };

        std::vector<Order> orders_by_security = getOrdersBySecurity(order.securityId());
        std::vector<Order> orders_by_company = getOrdersByCompany(order.company());
        std::vector<Order> orders_to_consider; //holds orders matching security and not company
        std::vector<Order> orders_to_process; //holds orders of the opposite side

        std::sort(orders_by_security.begin(), orders_by_security.end(), cmp);
        std::sort(orders_by_company.begin(), orders_by_company.end(), cmp);

        //find orders of the same security that is not for the same company
        std::set_difference(orders_by_security.begin(), orders_by_security.end(), orders_by_company.begin(), orders_by_company.end(), std::inserter(orders_to_consider, orders_to_consider.begin()), cmp);

        //remove same side orders
        std::remove_copy_if(begin(orders_to_consider), end(orders_to_consider),
            std::back_inserter(orders_to_process),
            [&](Order order_to_consider) { return order_to_consider.side() == order.side(); });

        std::vector<Order> orders_matched; //holds orders used in match
        for (auto order_to_process : orders_to_process) {
            if (order.qty_matched() == order.qty()) break;

            //get quantity used in match
            unsigned int qty_used = order_to_process.match(order);

            if (qty_used > 0) {
                order.qty_matched(order.qty_matched() + qty_used);

                //update result
                orders_matched.push_back(order_to_process);

                //update cache
                update(order);
                update(order_to_process);
            }
        }

        return orders_matched;
    }

private:
    std::unordered_set<Order> m_orders;
};
