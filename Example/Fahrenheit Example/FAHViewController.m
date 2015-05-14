//
//  FAHViewController.m
//  Fahrenheit
//
//  Created by Griffin Schneider on 03/29/2015.
//  Copyright (c) 2014 Griffin Schneider. All rights reserved.
//

#import "FAHViewController.h"
#import <Fahrenheit/Fahrenheit.h>

@interface FAHViewController ()

@end



//void takesInt(NSUInteger i) {
//    NSLog(@"one int");
//}
//void takesView(UIView *i) {
//    NSLog(@"one view");
//}
//#define TEST(x) _Generic((x), NSUInteger: takesInt, default: takesView)(x)


@implementation FAHViewController

- (void)loadView {
    
    [super loadView];
    UIEdgeInsets insets = UIEdgeInsetsMake(30, 30, 30, 30);
    
    ºº(self.view) {
        _.backgroundColor = [UIColor whiteColor];
        UIView* º(view1) {
            _.backgroundColor = [UIColor redColor];
            _.make.edges.equalTo(_.superview).with.insets(insets);
            UIView* º(view2) {
                _.backgroundColor = [UIColor greenColor];
                _.make.edges.equalTo(_.superview).with.insets(insets);
                UIView* º(view3) {
                    _.backgroundColor = [UIColor blueColor];
                    _.make.edges.equalTo(_.superview).with.insets(insets);
                    UIView* º(view4) {
                        _.backgroundColor = [UIColor orangeColor];
                        _.make.edges.equalTo(_.superview).with.insets(insets);
                    };
                };
            };
        };
    };
}

@end
