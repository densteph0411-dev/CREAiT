#include "reviewscontroller.h"

QMap<QString, ReviewItemData> ReviewsController::reviewItems = {
{"#123ABC1", { "#123ABC1", "Software Testing",
    {
        "2.0.9.1", "Software Test Mechanism", "Lorem ipsum dolor sit amet consectetur. Lectus et aliquam auctor ac lectus tortor aliquet.. Elementum etiam aliquet molestie id nulla.. Hendrerit proin fames feugiat amet urna vestibulum.. Dictumst ultrices leo duis facilisis amet.. Amet gravida facilisi adipiscing neque nam blandit imperdiet amet dictumst.. Diam arcu eget consectetur erat mi consectetur commodo.. ",
        "Draft", "System requirement",
        {
            "C:/Documents/Records/record1.xml",
            {
                {"Field Name1", "Value1"},
                {"Field Name2", "Value2"},
                {"Field Name3", "Value3"},
            }
        },
        {
            { "#4563AD", "Lerem Ipsum", "Down", "Link" },
            { "#4563AE", "Lerem Ipsum", "Up", "Link" },
        },
    },
    {
        "2.0.9.4", "Software Test Mechanism", "Lorem ipsum dolor sit amet consectetur. Lectus et aliquam auctor ac lectus tortor aliquet.. Elementum etiam aliquet molestie id nulla.. Hendrerit proin fames feugiat amet urna vestibulum.. Dictumst ultrices leo duis facilisis amet.. Amet gravida facilisi adipiscing neque nam blandit imperdiet amet dictumst.. Diam arcu eget consectetur erat mi consectetur commodo.. ",
        "Finaized", "System requirement",
        {
            "C:/Documents/Records/record2.xml",
            {
                {"Field Name1", "Value1"},
                {"Field Name2", "Value2"},
                {"Field Name3", "Value3"},
                {"Field Name4", "Value4"},
            }
        },
        {
            { "#6763AD", "Lerem Ipsum", "Up", "Link" },
            { "#6763AE", "Lerem Ipsum", "Up", "Link" },
        },
    },
    {
        {
            "#123CDH1", "Michael Brown", "Johnny Wick", "Verified", "", "James William",
            "",
            "Jul 12, 2025",
            {
                { "#123CDE3", "Michael Brown", "This is a comment for Finding of Review Item", "Jun 23, 2025"},
                { "#123CDE4", "Johnny Wick", "This is a comment for Finding of Review Item", "May 23, 2025"},
                { "#123CDE5", "James William", "This is a comment for Finding of Review Item", "Jul 12, 2025"},
            }
        },
        {
            "#123CDH2", "Michael Brown", "Johnny Wick", "Verified", "", "James William",
            "",
            "Mar 11, 2025",
            {
                { "#123CDE1", "Michael Brown", "This is a comment for Finding of Review Item", "Jun 23, 2025"},
                { "#123CDE2", "Johnny Wick", "This is a comment for Finding of Review Item", "May 23, 2025"},
            }
        },
    }
}},
{ "#123ABC2", {"#TY78JAZZ", "System Requirement",
    {
     "2.0.9.1", "System Requirement", "Lorem ipsum dolor sit amet consectetur. Lectus et aliquam auctor ac lectus tortor aliquet.. Elementum etiam aliquet molestie id nulla.. Hendrerit proin fames feugiat amet urna vestibulum.. Dictumst ultrices leo duis facilisis amet.. Amet gravida facilisi adipiscing neque nam blandit imperdiet amet dictumst.. Diam arcu eget consectetur erat mi consectetur commodo.. ",
     "Draft", "System requirement",
     {
         "C:/Documents/Records/record1.xml",
         {
          {"Field Name1", "Value1"},
          {"Field Name2", "Value2"},
          {"Field Name3", "Value3"},
          }
     },
     {
         { "#4563AD", "Lerem Ipsum", "Down", "Link" },
         { "#4563AE", "Lerem Ipsum", "Up", "Link" },
         },
     },
    {
     "2.0.9.6", "Software Test Mechanism", "Lorem ipsum dolor sit amet consectetur. Lectus et aliquam auctor ac lectus tortor aliquet.. Elementum etiam aliquet molestie id nulla.. Hendrerit proin fames feugiat amet urna vestibulum.. Dictumst ultrices leo duis facilisis amet.. Amet gravida facilisi adipiscing neque nam blandit imperdiet amet dictumst.. Diam arcu eget consectetur erat mi consectetur commodo.. ",
     "Finaized", "System requirement",
     {
         "C:/Documents/Records/record2.xml",
         {
          {"Field Name1", "Value1"},
          {"Field Name2", "Value2"},
          {"Field Name3", "Value3"},
          {"Field Name4", "Value4"},
          }
     },
     {
         { "#6763AD", "Lerem Ipsum", "Up", "Link" },
         { "#6763AE", "Lerem Ipsum", "Up", "Link" },
         },
     },
    {
     {
         "#223CDH1", "Michael Brown", "Johnny Wick", "Verified", "", "James William",
         "",
         "Jul 12, 2025",
         {
          { "#123CDE1", "Michael Brown", "This is a comment for Finding of Review Item", "Jun 23, 2025"},
          { "#123CDF1", "Johnny Wick", "This is a comment for Finding of Review Item", "May 23, 2025"},
          { "#123CDG1", "James William", "This is a comment for Finding of Review Item", "Jul 12, 2025"},
          }
     },
     {
         "#223CDH2", "Michael Brown", "Johnny Wick", "Verified", "", "James William",
         "",
         "Mar 11, 2025",
         {
          { "#123CDH1", "Michael Brown", "This is a comment for Finding of Review Item", "Jun 23, 2025"},
          { "#123CDI1", "Johnny Wick", "This is a comment for Finding of Review Item", "May 23, 2025"},
          }
     },
     }
},}};

QMap<QString, ReviewData> ReviewsController::reviews = {
{"#326BRE1", {"#326BRE1", "Tim Trvis", "Oct 11, 2025", "Michael Brown", "Michal Joe", 5, 2,
    ReviewsController::reviewItems,
    {
        {"Michal Joe", 2},
        {"James William", 1},
        {"Johnny Wick", 0}
    },
    { 1, 3, 8, 10, 15, 16 },
    { 3, 6, 7, 12, 13 },
    {
        {"Title1", "Description for title1 is optional"},
        {"Title2", "Description for title2 is optional"},
        {"Title3", "Description for title3 is optional"},
        {"Title4", "Description for title4 is optional"},
        {"Title5", "Description for title5 is optional"}
    }
}},
{"#326BRE2", {"#326BRE2", "James William", "May 03, 2025", "Michael Brown", "Tim Trvis", 4, 1,
    ReviewsController::reviewItems,
    {
        {"Tim Trvis", 2},
        {"Johnny Wick", 0},
        {"Michal Joe", 1}
    },
    { 2, 12, 13, 18, 19, 20 },
    { 4, 11, 16, 20 },
    {
        {"Title1", "Description for title1 is optional"},
        {"Title2", "Description for title2 is optional"},
        {"Title3", "Description for title3 is optional"},
        {"Title4", "Description for title4 is optional"},
        {"Title5", "Description for title5 is optional"}
    }
}},
};
