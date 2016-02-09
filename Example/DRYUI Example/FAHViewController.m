//
//  FAHViewController.m
//  DRYUI
//
//  Created by Griffin Schneider on 03/29/2015.
//  Copyright (c) 2014 Griffin Schneider. All rights reserved.
//

#import "FAHViewController.h"
#import <DRYUI/DRYUI.h>
#import "FAHAppDelegate.h"
#import "DRYUI_Example-swift.h"

dryui_style(Green, UIView) {
    _.backgroundColor = [UIColor greenColor];
};
dryui_style(Blue, UIView) {
    _.backgroundColor = [UIColor blueColor];
};
dryui_style(Orange, UIButton) {
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
                    UILabel* add_subview(view4, [SwiftStyles aStyle:@"thing"], Blue) {
                        _.make.edges.equalTo(_.superview).with.insets(insets);
                    };
                    UILabel *add_subview(view5, ^(UIView *_) {
                        
                    });
                };
            };
        };
    };
}

@end
