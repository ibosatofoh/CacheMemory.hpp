# CacheMemory.hpp
This code is an implementation of a cache memory for storing and managing orders. It includes several functions for adding, removing, updating, and retrieving orders based on various criteria such as security ID, company, and user.

The CacheMemory class implements get, getOrders, getOrdersBySecurity, getOrdersByCompany, getOrdersByUser and getMatches methods of the CacheBackend class. The getMatches method is used to find matching orders for a given order.

This repository contains C++ code that defines a class CacheMemory which is a concrete implementation of the abstract class CacheBackend.

CacheBackend defines a set of methods for manipulating and retrieving Order objects, while CacheMemory provides an implementation of these methods using an in-memory std::unordered_set to store the orders.

The code uses C++ features such as inheritance, virtual functions, iterators, lambda functions, and standard library algorithms to provide an efficient implementation for the cache.
