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

@implementation FAHViewController

- (void)loadView {
    [super loadView];
    UIEdgeInsets insets = UIEdgeInsetsMake(30, 30, 30, 30);
    ºº(self.view) {
        _.backgroundColor = [UIColor whiteColor];
        º(UIView) {
            _.backgroundColor = [UIColor redColor];
            _.make.edges.equalTo(_.superview).with.insets(insets);
            º(UIView) {
                _.backgroundColor = [UIColor greenColor];
                _.make.edges.equalTo(_.superview).with.insets(insets);
                º(UIView) {
                    _.backgroundColor = [UIColor blueColor];
                    _.make.edges.equalTo(_.superview).with.insets(insets);
                    º(UIView) {
                        _.backgroundColor = [UIColor orangeColor];
                        _.make.edges.equalTo(_.superview).with.insets(insets);
                    };
                };
            };
        };
    };
}

@end
