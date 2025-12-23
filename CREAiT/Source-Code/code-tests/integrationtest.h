#pragma once

#include <iostream>

class integrationTest
{
public:
    integrationTest() {
        // Add Constructor
    }
    ~integrationTest() {
        // If dictionary is allocated dynamically, clean it up here.
        // For example:
        // delete dictionary;
        // (Alternatively, consider using smart pointers.)
    }

    // Run all tests
    void runAllTests(void);


	//-- Project Tests ------------------------------------------
    
	// Run all project tests
    bool runAllProjectTests(void);

    // Test Creating a project
    bool createProjectTest(void);

    // Test Closing the project
    bool closeProjectTest(void);

    // Test Opening the project
    bool openProjectTest(void);

    // Test to create another project
    bool createAnotherProjectTest(void);

    // Test to create another project
    bool createManyProjectTests(void);
	
    // Test to create another project
    bool getRecentProjectsTest(void);
	
    // Test to create another project
    bool changeToAllProjectsTest(void);
	
	//-- Record Tests ------------------------------------------
	
	// Test that creates one record
    bool createRecordTest(void);	

	// Test that creates eachRecordType
    bool createEachRecordTypeTest(void);	
	
	// Test that creates many records
    bool createManyRecordsTest(void);	
	
	// Test that creates many records
    bool removeRecordsTest(void);
	
	// Test that creates many records
    bool addLinksToRecordTest(void);
	
	// Test that creates many records
    bool removeLinksToRecordTest(void);
	
	// Test that creates many records
    bool createManyLinksTest(void);

    // Get RecordTypesTest
    bool getRecordTypes (void);

    // Tests Automations functionality
    bool createManyAutomationsTest(void);

    // Tests Reviews functionality
    bool createManyReviewsTest(void);

    // Test recordTypePaths
    bool testRecordTypePaths (void);

    // Test integration creations
    bool createIntegrations(void);

    // Test document creation data
    bool createDocData(void);

    bool createDocTest(std::string ID, std::string title, std::string description);

    // Test user creation data
    bool createUserData(void);
};

