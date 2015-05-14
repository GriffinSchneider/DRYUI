//
//  FahrenheitTests.m
//  FahrenheitTests
//
//  Created by Griffin Schneider on 03/29/2015.
//  Copyright (c) 2014 Griffin Schneider. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>
#import <Fahrenheit/Fahrenheit.h>


NS_ENUM(NSUInteger, ExampleStyleEnum) {
    Style0,
    Style1,
    Style2,
    Style3,
};

@interface FahrenheitTests : XCTestCase

@end

@implementation FahrenheitTests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)testFahrenheit {

    UIView *topLevel = [UIView new];
    __block UIView *a, *b, *c, *d, *e, *f;
    __block UIButton *g, *gg;
    
    ºº(topLevel) {
        _.make.edges.equalTo(_);
        º(a, Style0, Style1, Style2, Style3) {
            XCTAssertNotNil(b, @"b should already be assigned when this block is run");
            XCTAssertNotNil(c, @"c should already be assigned when this block is run");
        };
        º(b, Style3) {
            [_ make];
            XCTAssertEqual(_.superview, superview, @"superview should be bound to view.superview");
            UIView* º(x) {
                _.tag = 2;
            };
        };
        º(c) {
            [_ make];
            º(d) {
                [_ make];
                XCTAssertEqual(_.superview, superview, @"superview should be bound to view.superview");
                º(e){
                    [_ make];
                    XCTAssertNotNil(b, @"b should already be assigned when this block is run");
                    XCTAssertNotNil(c, @"c should already be assigned when this block is run");
                    XCTAssertNotNil(f, @"f should already be assigned when this block is run");
                    XCTAssertNotNil(g, @"g should already be assigned when this block is run");
                };
                º(f){};
                º(g, ({gg = [UIButton buttonWithType:UIButtonTypeCustom];}), Style0, Style1) {
                    XCTAssertEqual(_, gg);
                    [_ make];
                    _.tag = 3;
                };
            };
        };
    };
    
    XCTAssertEqual(a.superview, topLevel, @"a's superview should be the top view");
    XCTAssertEqual(b.superview, topLevel, @"b's superview should be the top view");
    XCTAssertEqual(c.superview, topLevel, @"c's superview should be the top view");
    XCTAssertEqualObjects(topLevel.subviews, (@[a, b, c]), @"a, b, and c should be the only subviews of the top view");
    
    XCTAssertEqual(c.subviews.count, 1, @"c should have 1 subview");
    XCTAssertEqual(c.subviews[0], d, @"c's subview should be d");
    XCTAssertEqual(d.subviews.count, 3, @"d should have 3 subviews");
    XCTAssertEqual([topLevel viewWithTag:2].superview, b, @"the view with tag 2 should be a subview of b");
    XCTAssertEqual([topLevel viewWithTag:3].superview, d, @"the view with tag 3 should be a subview of d");
    
    XCTAssertTrue(a.translatesAutoresizingMaskIntoConstraints, @"views that don't use _.make shouldn't set translatesAutoresizingMaskIntoConstraints to NO");
    XCTAssertTrue(f.translatesAutoresizingMaskIntoConstraints, @"views that don't use _.make shouldn't set translatesAutoresizingMaskIntoConstraints to NO");
    
    XCTAssertFalse(b.translatesAutoresizingMaskIntoConstraints, @"views that use _.make should set translatesAutoresizingMaskIntoConstraints to NO");
    XCTAssertFalse(c.translatesAutoresizingMaskIntoConstraints, @"views that use _.make should set translatesAutoresizingMaskIntoConstraints to NO");
    XCTAssertFalse(d.translatesAutoresizingMaskIntoConstraints, @"views that use _.make should set translatesAutoresizingMaskIntoConstraints to NO");
    XCTAssertFalse(e.translatesAutoresizingMaskIntoConstraints, @"views that use _.make should set translatesAutoresizingMaskIntoConstraints to NO");
    XCTAssertFalse(g.translatesAutoresizingMaskIntoConstraints, @"views that use _.make should set translatesAutoresizingMaskIntoConstraints to NO");
}

@end
