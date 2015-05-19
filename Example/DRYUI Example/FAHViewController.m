//
//  FAHViewController.m
//  DRYUI
//
//  Created by Griffin Schneider on 03/29/2015.
//  Copyright (c) 2014 Griffin Schneider. All rights reserved.
//

#import "FAHViewController.h"
#import <DRYUI/DRYUI.h>

dryui_private_style(Red) {
    _.backgroundColor = [UIColor redColor];
};
dryui_private_style(Green) {
    _.backgroundColor = [UIColor greenColor];
};
dryui_private_style(Blue) {
    _.backgroundColor = [UIColor blueColor];
};
dryui_private_style(Orange) {
    _.backgroundColor = [UIColor orangeColor];
};


@interface FAHViewController ()

@end

@implementation FAHViewController
    
- (void)loadView {
    
    [super loadView];
    UIEdgeInsets insets = UIEdgeInsetsMake(30, 30, 30, 30);
    
    build_subviews(self.view) {
        _.backgroundColor = [UIColor whiteColor];
        UIView* add_subview(view1, Red) {
            _.make.edges.equalTo(_.superview).with.insets(insets);
            UIView* add_subview(view2, Green) {
                _.make.edges.equalTo(_.superview).with.insets(insets);
                UIView* add_subview(view3, Blue) {
                    _.make.edges.equalTo(_.superview).with.insets(insets);
                    UIView* add_subview(view4, Orange) {
                        _.make.edges.equalTo(_.superview).with.insets(insets);
                    };
                };
            };
        };
    };
}

@end
