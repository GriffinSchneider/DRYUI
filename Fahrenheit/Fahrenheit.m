//
//  Fahrenheit.m
//
//  Created by Griffin Schneider on 3/3/15.
//  Copyright (c) 2015 Griffin Schneider. All rights reserved.
//

#import "Fahrenheit.h"
#import <objc/runtime.h>


static const char fahrenheit_constraintMakerId = 0;
static const char fahrenheit_addBlocksId = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////
@interface FAHRENHEIT_VIEW (Fahrenheit_Private)

@property (nonatomic, strong) MASConstraintMaker *constraintMaker;
@property (nonatomic, strong) NSMutableArray *addBlocks;

@end

@implementation FAHRENHEIT_VIEW (Fahrenheit_Private)

- (MASConstraintMaker *)constraintMaker {
    return objc_getAssociatedObject(self, &fahrenheit_constraintMakerId);
}
- (void)setConstraintMaker:(MASConstraintMaker *)constraintMaker {
    objc_setAssociatedObject(self, &fahrenheit_constraintMakerId, constraintMaker, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (NSMutableArray *)addBlocks {
    return objc_getAssociatedObject(self, &fahrenheit_addBlocksId);
}
- (void)setAddBlocks:(NSMutableArray *)addBlocks {
    objc_setAssociatedObject(self, &fahrenheit_addBlocksId, addBlocks, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

@end


////////////////////////////////////////////////////////////////////////////////////////////////////
@implementation FAHRENHEIT_VIEW (Fahrenheit)

- (void)buildSubviews:(FahrenheitBuildSubviewsBlock)block {
    self.addBlocks = [NSMutableArray array];
    block(self, ^(FAHRENHEIT_VIEW *view, FAHRENHEIT_VIEW *superview, FahrenheitAddArgumentBlock block) {
        return [self addViewFromBuildSubviews:view withSuperview:superview andBlock:block];
    });
    [self runAllAddBlocks];
}

- (id)addViewFromBuildSubviews:(FAHRENHEIT_VIEW *)view withSuperview:(FAHRENHEIT_VIEW *)superview andBlock:(FahrenheitAddArgumentBlock)block {
    [view setTranslatesAutoresizingMaskIntoConstraints:NO];
    [superview addSubview:view];
    
    // Don't actually need to weakify these references since the block will get released
    // after it's run, but we get a block-retain-cycle warning without weakification.
    __weak FAHRENHEIT_VIEW *weakSuperview = superview;
    __weak FAHRENHEIT_VIEW *weakView = view;
    
    // Add a block to view's superview's block list that does some setup, calls the given block, and then runs
    // all the blocks in view's block list (in order to run any blocks put there by calls to this method
    // from the given block).
    // view's superview's block list will eventually get run by the call to this method (or buildSubviews) that
    // recursed into this one.
    if (block) {
        [superview.addBlocks addObject:^{
            weakView.constraintMaker = [[MASConstraintMaker alloc] initWithView:weakView];
            weakView.addBlocks = [NSMutableArray array];
            block(weakView, weakSuperview);
            [weakView.constraintMaker install];
            weakView.constraintMaker = nil;
            [weakView runAllAddBlocks];
        }];
    }
    return view;
}

- (void)runAllAddBlocks {
    for (void(^block)() in self.addBlocks) {
        block();
    }
    self.addBlocks = nil;
}

#define __FAHRENHEIT_VIEW_STRING __FAHRENHEIT_VIEW_STRING_HELPER(FAHRENHEIT_VIEW)
#define __FAHRENHEIT_VIEW_STRING_HELPER(x) @#x
- (MASConstraintMaker *)make {
    NSAssert(self.constraintMaker != nil, @"%@.make should only be used inside a call to buildSubviews.", __FAHRENHEIT_VIEW_STRING);
    return self.constraintMaker;
}


@end


////////////////////////////////////////////////////////////////////////////////////////////////////
@implementation NSObject (Fahrenheit)

+ (instancetype)fahrenheit_selfOrInstanceOfSelf {
    return [self new];
}
- (instancetype)fahrenheit_selfOrInstanceOfSelf {
    return self;
}

@end

