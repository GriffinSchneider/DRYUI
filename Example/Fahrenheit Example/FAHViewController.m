//
//  FAHViewController.m
//  Fahrenheit
//
//  Created by Griffin Schneider on 03/29/2015.
//  Copyright (c) 2014 Griffin Schneider. All rights reserved.
//

#import "FAHViewController.h"
#import <Fahrenheit/DRYUI.h>

DRYUI_IMPLEMENT_STYLE(Red) { _.backgroundColor = [UIColor redColor]; };
DRYUI_IMPLEMENT_STYLE(Green) { _.backgroundColor = [UIColor greenColor]; };
DRYUI_IMPLEMENT_STYLE(Blue) { _.backgroundColor = [UIColor blueColor]; };
DRYUI_IMPLEMENT_STYLE(Orange) { _.backgroundColor = [UIColor orangeColor]; };

@interface FAHViewController ()

@end

@implementation FAHViewController
    
- (void)loadView {
    
    [super loadView];
    UIEdgeInsets insets = UIEdgeInsetsMake(30, 30, 30, 30);
    
    ºº(self.view) {
        _.backgroundColor = [UIColor whiteColor];
        UIView* º(view1, Red) {
            _.make.edges.equalTo(_.superview).with.insets(insets);
            UIView* º(view2, Green) {
                _.make.edges.equalTo(_.superview).with.insets(insets);
                UIView* º(view3, Blue) {
                    _.make.edges.equalTo(_.superview).with.insets(insets);
                    UIView* º(view4, Orange) {
                        _.make.edges.equalTo(_.superview).with.insets(insets);
                    };
                };
            };
        };
    };
}

@end
