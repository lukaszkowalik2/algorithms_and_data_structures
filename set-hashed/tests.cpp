#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <functional>

#include "set-hashed.h"
#include "utilities.h"

bool compare_element_vectors(std::vector<std::string> v1, std::vector<std::string> v2) {
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
    return v1 == v2;
}

void test_result(bool condition, const std::string& test_name) {
    std::cout << test_name << ": " << (condition ? "PASSED" : "FAILED") << std::endl;
}

int main() {
    
    // Basic operations tests
    setHashed s1(5);
    s1.insert("Test"); s1.insert("Data"); s1.insert("AAAA"); s1.insert("BBBB");
    test_result(s1.size() == 4, "Initial size after inserts");
    test_result(s1.contains("Test"), "Contains existing element");
    test_result(!s1.contains("ZZZZ"), "Does not contain non-existent element");

    // Duplicate insert test
    s1.insert("Test");
    test_result(s1.size() == 4, "Size unchanged after duplicate insert");
    
    // Remove tests
    s1.remove("Data");
    test_result(s1.size() == 3, "Size after remove");
    test_result(!s1.contains("Data"), "Element removed successfully");
    s1.remove("XXXX");
    test_result(s1.size() == 3, "Size unchanged after removing non-existent element");

    // Set operations tests
    setHashed s2(5);
    s2.insert("AAAA"); s2.insert("CCCC"); s2.insert("DDDD");

    // Union test
    setHashed s_union = setHashed::setUnion(s1, s2);
    std::vector<std::string> expected_union = {"AAAA", "BBBB", "CCCC", "DDDD", "Test"};
    test_result(compare_element_vectors(s_union.get_elements(), expected_union), "Union operation");
    test_result(s_union.size() == 5, "Union size");

    // Intersection test
    setHashed s_intersection = setHashed::intersection(s1, s2);
    std::vector<std::string> expected_intersection = {"AAAA"};
    test_result(compare_element_vectors(s_intersection.get_elements(), expected_intersection), "Intersection operation");
    test_result(s_intersection.size() == 1, "Intersection size");

    // Difference tests
    setHashed s_diff1 = setHashed::difference(s1, s2);
    std::vector<std::string> expected_diff1 = {"BBBB", "Test"};
    test_result(compare_element_vectors(s_diff1.get_elements(), expected_diff1), "Difference s1-s2");
    test_result(s_diff1.size() == 2, "Difference s1-s2 size");
    
    setHashed s_diff2 = setHashed::difference(s2, s1);
    std::vector<std::string> expected_diff2 = {"CCCC", "DDDD"};
    test_result(compare_element_vectors(s_diff2.get_elements(), expected_diff2), "Difference s2-s1");
    test_result(s_diff2.size() == 2, "Difference s2-s1 size");

    // Equality tests
    setHashed s3(5); s3.insert("Test"); s3.insert("AAAA"); s3.insert("BBBB");
    test_result(setHashed::equals(s1, s3), "Equal sets with same table size");
    test_result(!setHashed::equals(s1, s2), "Unequal sets");
    
    // Different table sizes but same elements
    setHashed s4(10); s4.insert("Test"); s4.insert("AAAA"); s4.insert("BBBB");
    test_result(setHashed::equals(s1, s4), "Equal sets with different table sizes");
    
    // Empty set tests
    setHashed empty1(5), empty2(10);
    test_result(setHashed::equals(empty1, empty2), "Empty sets equality");
    test_result(empty1.size() == 0, "Empty set size");
    
    // Operations with empty set
    setHashed s5(5); s5.insert("AAAA"); s5.insert("BBBB");
    setHashed union_empty = setHashed::setUnion(s5, empty1);
    test_result(compare_element_vectors(union_empty.get_elements(), s5.get_elements()), "Union with empty set");
    
    setHashed intersect_empty = setHashed::intersection(s5, empty1);
    test_result(intersect_empty.size() == 0, "Intersection with empty set");
    
    setHashed diff_empty = setHashed::difference(s5, empty1);
    test_result(compare_element_vectors(diff_empty.get_elements(), s5.get_elements()), "Difference with empty set");
    
    
    return 0;
}
