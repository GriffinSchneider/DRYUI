//
//  FahrenheitTests.m
//  FahrenheitTests
//
//  Created by Griffin Schneider on 03/29/2015.
//  Copyright (c) 2014 Griffin Schneider. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>
#import <Fahrenheit/DRYUI.h>

#define BIG_STYLE_LIST \
Style0, Style1, Style2, Style3, Style3, Style3, \
Style3, Style3, Style3, Style3, Style3, Style3, \
Style3, Style3, Style3, Style3, Style3, Style3, \
Style3, Style3, Style3, Style3, Style3, Style3, \
Style3, Style3, Style3, Style3, Style3, Style3, \
Style3 \

@interface FahrenheitTests : XCTestCase

@end

@implementation FahrenheitTests

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

// Can't use XCTAssert without `self`, so we have to save booleans from
// these blocks and assert later instead of asserting in the blocks themselves
static BOOL wasSuperviewEverNil = NO;

DRYUI_IMPLEMENT_STYLE(Style0) {
    if (!superview) wasSuperviewEverNil = YES;
    _.backgroundColor = [UIColor redColor];
};

DRYUI_IMPLEMENT_STYLE(Style1) {
    if (!superview) wasSuperviewEverNil = YES;
    _.backgroundColor = [UIColor blueColor];
};

DRYUI_IMPLEMENT_STYLE(Style2) {
    if (!superview) wasSuperviewEverNil = YES;
    _.backgroundColor = [UIColor greenColor];
};

DRYUI_IMPLEMENT_STYLE(Style3) {
    if (!superview) wasSuperviewEverNil = YES;
    _.backgroundColor = [UIColor orangeColor];
};

DRYUI_IMPLEMENT_STYLE(StyleButton, UIButton) {
    [_ setTitle:@"button title" forState:UIControlStateNormal];
};

- (void)testFahrenheit {

    UIView *topLevel = [UIView new];
    __block UIView *a, *b, *c, *d, *e, *f;
    __block UIButton *g, *gg;
    
    ºº(topLevel) {
        _.make.edges.equalTo(_);
        º(a, BIG_STYLE_LIST) {
            XCTAssertNotNil(b, @"b should already be assigned when this block is run");
            XCTAssertNotNil(c, @"c should already be assigned when this block is run");
        };
        º(b, Style3) {
            [_ make];
            XCTAssertEqual(_.superview, superview, @"superview should be bound to view.superview");
            _.backgroundColor = [UIColor purpleColor];
            UIView* º(x) {
                _.tag = 2;
            };
        };
        º(c, Style0, Style1) {
            [_ make];
            º(d) {
                [_ make];
                XCTAssertEqual(_.superview, superview, @"superview should be bound to view.superview");
                º(e) {
                    [_ make];
                    XCTAssertNotNil(b, @"b should already be assigned when this block is run");
                    XCTAssertNotNil(c, @"c should already be assigned when this block is run");
                    XCTAssertNotNil(f, @"f should already be assigned when this block is run");
                    XCTAssertNotNil(g, @"g should already be assigned when this block is run");
                };
                º(f){};
                º(g, ({gg = [UIButton buttonWithType:UIButtonTypeCustom];}), StyleButton, Style1) {
                    XCTAssertEqual(_, gg);
                    [_ make];
                    _.tag = 3;
                };
            };
        };
    };
    
    // Assertions about hierarchy
    XCTAssertEqual(a.superview, topLevel, @"a's superview should be the top view");
    XCTAssertEqual(b.superview, topLevel, @"b's superview should be the top view");
    XCTAssertEqual(c.superview, topLevel, @"c's superview should be the top view");
    XCTAssertEqualObjects(topLevel.subviews, (@[a, b, c]), @"a, b, and c should be the only subviews of the top view");
    
    XCTAssertEqual(c.subviews.count, 1, @"c should have 1 subview");
    XCTAssertEqual(c.subviews[0], d, @"c's subview should be d");
    XCTAssertEqual(d.subviews.count, 3, @"d should have 3 subviews");
    XCTAssertEqual([topLevel viewWithTag:2].superview, b, @"the view with tag 2 should be a subview of b");
    XCTAssertEqual([topLevel viewWithTag:3].superview, d, @"the view with tag 3 should be a subview of d");
    
    // Assertions about layout constraints
    XCTAssertTrue(a.translatesAutoresizingMaskIntoConstraints, @"views that don't use _.make shouldn't set translatesAutoresizingMaskIntoConstraints to NO");
    XCTAssertTrue(f.translatesAutoresizingMaskIntoConstraints, @"views that don't use _.make shouldn't set translatesAutoresizingMaskIntoConstraints to NO");
    
    XCTAssertFalse(b.translatesAutoresizingMaskIntoConstraints, @"views that use _.make should set translatesAutoresizingMaskIntoConstraints to NO");
    XCTAssertFalse(c.translatesAutoresizingMaskIntoConstraints, @"views that use _.make should set translatesAutoresizingMaskIntoConstraints to NO");
    XCTAssertFalse(d.translatesAutoresizingMaskIntoConstraints, @"views that use _.make should set translatesAutoresizingMaskIntoConstraints to NO");
    XCTAssertFalse(e.translatesAutoresizingMaskIntoConstraints, @"views that use _.make should set translatesAutoresizingMaskIntoConstraints to NO");
    XCTAssertFalse(g.translatesAutoresizingMaskIntoConstraints, @"views that use _.make should set translatesAutoresizingMaskIntoConstraints to NO");
    
    
    // Assertions about style association
    NSMutableArray *styles = [NSMutableArray new];
    const _DRYUIStyle *_styles[] = {BIG_STYLE_LIST};
    for (int i = 0; i < sizeof(_styles)/sizeof(_DRYUIStyle*); i++) {
        [styles addObject:[NSString stringWithUTF8String:_styles[i]->name]];
    }
    XCTAssertEqualObjects(a.styleNames, styles, @"a's styles should equal the BIG_STYLE_LIST");
    
    XCTAssertEqualObjects(b.styleNames, @[[NSString stringWithUTF8String:Style3->name]], @"b's styles should equal [Style3]");
    XCTAssertEqualObjects(c.styleNames, (@[[NSString stringWithUTF8String:Style0->name], [NSString stringWithUTF8String:Style1->name]]), @"c's styles should equal [Style0, Style1]");
    XCTAssertEqualObjects(g.styleNames, (@[[NSString stringWithUTF8String:StyleButton->name], [NSString stringWithUTF8String:Style1->name]]), @"g's styles should equal [StyleButton, Style1]");
    
    XCTAssertNil(d.styleNames, @"d shouldn't have any styles");
    XCTAssertNil(e.styleNames, @"d shouldn't have any styles");
    XCTAssertNil(f.styleNames, @"d shouldn't have any styles");
    
    
    // Assertions about style application
    XCTAssertFalse(wasSuperviewEverNil, @"superview should never be nil when applying a style");
    XCTAssertEqual(a.backgroundColor, [UIColor orangeColor], @"a should be orange");
    XCTAssertEqual(b.backgroundColor, [UIColor purpleColor], @"b should be purple");
    XCTAssertEqual(c.backgroundColor, [UIColor blueColor], @"c should be blue");
    XCTAssertEqual(g.backgroundColor, [UIColor blueColor], @"g should be blue");
    XCTAssertEqual([g titleForState:UIControlStateNormal], @"button title");
}

@end
