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

@property UIView *f;

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

dryui_style(Style0) {
    if (!superview) wasSuperviewEverNil = YES;
    _.backgroundColor = [UIColor redColor];
};

dryui_style(Style1) {
    parent_style(Style0);
    if (!superview) wasSuperviewEverNil = YES;
    _.backgroundColor = [UIColor blueColor];
};

dryui_style(Style2) {
    parent_style(Style1);
    if (!superview) wasSuperviewEverNil = YES;
    _.backgroundColor = [UIColor greenColor];
};

dryui_style(Style3) {
    parent_style(Style2);
    if (!superview) wasSuperviewEverNil = YES;
    _.backgroundColor = [UIColor orangeColor];
};

dryui_style(StyleButton, UIButton) {
    parent_style(Style0);
    [_ setTitle:@"button title" forState:UIControlStateNormal];
    parent_style(Style1);
    parent_style(Style3);
};

- (void)testFahrenheit {

    UIView *topLevel = [UIView new];
    __block UIView *a, *b, *c, *d, *e;
    __block UIButton *g, *gg;
    
    __block UIButton *h = [UIButton new];
    UIButton *hh = h;
    
    build_subviews(topLevel) {
        _.make.edges.equalTo(_);
        add_subview(a, BIG_STYLE_LIST) {
            XCTAssertNotNil(b, @"b should already be assigned when this block is run");
            XCTAssertNotNil(c, @"c should already be assigned when this block is run");
        };
        add_subview(b, Style3) {
            [_ make];
            XCTAssertEqual(_.superview, superview, @"superview should be bound to view.superview");
            _.backgroundColor = [UIColor purpleColor];
            UIView* add_subview(x) {
                _.tag = 2;
            };
        };
        add_subview(c, Style0, Style1) {
            [_ make];
            add_subview(d) {
                [_ make];
                XCTAssertEqual(_.superview, superview, @"superview should be bound to view.superview");
                add_subview(e) {
                    [_ make];
                    XCTAssertNotNil(b, @"b should already be assigned when this block is run");
                    XCTAssertNotNil(c, @"c should already be assigned when this block is run");
                    XCTAssertNotNil(self.f, @"self.f should already be assigned when this block is run");
                    XCTAssertNotNil(g, @"g should already be assigned when this block is run");
                };
                add_subview(self.f){};
                add_subview(g, ({gg = [UIButton buttonWithType:UIButtonTypeSystem];}), Style1, StyleButton) {
                    XCTAssertEqual(_, gg);
                    [_ make];
                    _.tag = 3;
                };
            };
        };
        add_subview(h) {
            XCTAssertEqual(h, hh, @"Using add_subview with a variable that's already assigned shouldn't re-assign the variable");
        };
    };
    
    // Assertions about hierarchy
    XCTAssertEqual(a.superview, topLevel, @"a's superview should be the top view");
    XCTAssertEqual(b.superview, topLevel, @"b's superview should be the top view");
    XCTAssertEqual(c.superview, topLevel, @"c's superview should be the top view");
    XCTAssertEqual(h.superview, topLevel, @"h's superview should be the top view");
    XCTAssertEqualObjects(topLevel.subviews, (@[a, b, c, h]), @"a, b, c, and h should be the only subviews of the top view");
    
    XCTAssertEqual(c.subviews.count, 1, @"c should have 1 subview");
    XCTAssertEqual(c.subviews[0], d, @"c's subview should be d");
    XCTAssertEqual(d.subviews.count, 3, @"d should have 3 subviews");
    XCTAssertEqual([topLevel viewWithTag:2].superview, b, @"the view with tag 2 should be a subview of b");
    XCTAssertEqual([topLevel viewWithTag:3].superview, d, @"the view with tag 3 should be a subview of d");
    
    // Assertions about layout constraints
    XCTAssertTrue(a.translatesAutoresizingMaskIntoConstraints, @"views that don't use _.make shouldn't set translatesAutoresizingMaskIntoConstraints to NO");
    XCTAssertTrue(self.f.translatesAutoresizingMaskIntoConstraints, @"views that don't use _.make shouldn't set translatesAutoresizingMaskIntoConstraints to NO");
    
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
    XCTAssertEqualObjects(g.styleNames, (@[[NSString stringWithUTF8String:Style1->name], [NSString stringWithUTF8String:StyleButton->name]]), @"g's styles should equal [StyleButton, Style1]");
    
    XCTAssertNil(d.styleNames, @"d shouldn't have any styles");
    XCTAssertNil(e.styleNames, @"d shouldn't have any styles");
    XCTAssertNil(self.f.styleNames, @"d shouldn't have any styles");
    
    
    // Assertions about style application
    XCTAssertFalse(wasSuperviewEverNil, @"superview should never be nil when applying a style");
    XCTAssertEqual(a.backgroundColor, [UIColor orangeColor], @"a should be orange");
    XCTAssertEqual(b.backgroundColor, [UIColor purpleColor], @"b should be purple");
    XCTAssertEqual(c.backgroundColor, [UIColor blueColor], @"c should be blue");
    XCTAssertEqual(g.backgroundColor, [UIColor orangeColor], @"g should be orange");
    XCTAssertEqual([g titleForState:UIControlStateNormal], @"button title");
}

@end
