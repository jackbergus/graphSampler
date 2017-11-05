//
// Created by Giacomo Bergami on 16/08/16.
//

#pragma once

/**
 * Defines the current status of the iteration
 */
enum class PositionIteratorEnum : char {
    Left = -1, Center = 0 ,Right = 1, None = -2
};

/**
 * Defines the color of the current subtree's root
 */
enum class Color { RED, BLACK };