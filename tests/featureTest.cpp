/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   featureTest.cpp
 * Author: lars
 *
 * Created on August 18, 2020, 6:36 AM
 */

#include <stdlib.h>
#include <iostream>

/*
 * Simple C++ Test Suite
 */

void test1() {
    std::cout << "featureTest test 1" << std::endl;
}

void test2() {
    std::cout << "featureTest test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (featureTest) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% featureTest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (featureTest)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (featureTest)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (featureTest)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (featureTest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

