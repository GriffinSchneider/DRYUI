//
//  Fahrenheit.m
//
//  Created by Griffin Schneider on 3/3/15.
//  Copyright (c) 2015 Griffin Schneider. All rights reserved.
//

#import "Fahrenheit.h"
#import <objc/runtime.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
@interface _FAHRENHEIT_VIEW (Fahrenheit_Private)

@property (nonatomic, strong) MASConstraintMaker *constraintMaker;
@property (nonatomic, strong) NSMutableArray *addBlocks;

@end

@implementation _FAHRENHEIT_VIEW (Fahrenheit_Private)

static const char fahrenheit_constraintMakerId = 0;
static const char fahrenheit_addBlocksId = 0;

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


id _fahrenheit_instantiate_from_encoding(char *encoding) {
    NSString *encodingString = [NSString stringWithUTF8String:encoding];
    
    NSRange braceRange = [encodingString rangeOfString:@"{"];
    NSRange equalsRange = [encodingString rangeOfString:@"="];
    NSString *className = [encodingString substringWithRange:NSMakeRange(braceRange.location+braceRange.length, equalsRange.location-1)];
    
    id instance = [NSClassFromString(className) new];
    
    return instance;
}

id _fahrenheit_takeStyleAndReturnNil(DRYUIStyle notView) {
    return nil;
}

id _fahrenheit_returnGivenView(UIView *view) {
    return view;
}

const DRYUIStyle _fahrenheit_returnGivenStyle(const DRYUIStyle style) {
    return style;
}

const DRYUIStyle _fahrenheit_takeViewAndReturnEmptyStyle(UIView *notAStyle) {
    return DRYUIEmptyStyle;
}

void _fahrenheit_applyStyleToView(UIView *view, DRYUIStyle style) {
    if (style.name == DRYUIEmptyStyle.name) {
        return;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////
@implementation _FAHRENHEIT_VIEW (Fahrenheit)

- (void)setupForFahrenheitAddBlock {
    self.constraintMaker = [[MASConstraintMaker alloc] initWithView:self];
    self.addBlocks = [NSMutableArray array];
}

- (void)tearDownAfterFahrenheitAddBlock {
    [self.constraintMaker install];
    self.constraintMaker = nil;
    [self runAllAddBlocks];
}

- (void)_fahrenheit_buildSubviews:(FahrenheitViewAndSuperviewBlock)block {
    [self setupForFahrenheitAddBlock];
    block(self, self.superview);
    [self tearDownAfterFahrenheitAddBlock];
}

- (id)_fahrenheit_addViewFromBuildSubviews:(_FAHRENHEIT_VIEW *)view withSuperview:(_FAHRENHEIT_VIEW *)superview andBlock:(FahrenheitViewAndSuperviewBlock)block {
    [superview addSubview:view];
    
    // Don't actually need to weakify these references since the block will get released
    // after it's run, but we get a block-retain-cycle warning without weakification.
    __weak _FAHRENHEIT_VIEW *weakSuperview = superview;
    __weak _FAHRENHEIT_VIEW *weakView = view;
    
    // Add a block to view's superview's block list that does some setup, calls the given block, and then runs
    // all the blocks in view's block list (in order to run any blocks put there by calls to this method
    // from the given block).
    // view's superview's block list will eventually get run by the call to this method (or buildSubviews) that
    // recursed into this one.
    if (block) {
        [superview.addBlocks addObject:^{
            [weakView setupForFahrenheitAddBlock];
            block(weakView, weakSuperview);
            [weakView tearDownAfterFahrenheitAddBlock];
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

#define _FAHRENHEIT_VIEW_STRING _FAHRENHEIT_VIEW_STRING_HELPER(_FAHRENHEIT_VIEW)
#define _FAHRENHEIT_VIEW_STRING_HELPER(x) @#x
- (MASConstraintMaker *)make {
    NSAssert(self.constraintMaker != nil, @"%@.make should only be used inside a call to buildSubviews.", _FAHRENHEIT_VIEW_STRING);
    [self setTranslatesAutoresizingMaskIntoConstraints:NO];
    return self.constraintMaker;
}


@end


////////////////////////////////////////////////////////////////////////////////////////////////////
@implementation NSObject (Fahrenheit)

+ (instancetype)_fahrenheit_selfOrInstanceOfSelf {
    return [self new];
}
- (instancetype)_fahrenheit_selfOrInstanceOfSelf {
    return self;
}

@end

