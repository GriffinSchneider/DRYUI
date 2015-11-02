//
//  DRYUI.m
//
//  Created by Griffin Schneider on 3/3/15.
//  Copyright (c) 2015 Griffin Schneider. All rights reserved.
//

#import "DRYUI.h"
#import <objc/runtime.h>


static const char dryui_constraintMakerId = 0;
static const char dryui_stylesId = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////
@implementation _DRYUI_VIEW (DRYUI)

- (MASConstraintMaker *)make {
    NSAssert(self._dryuiConstraintMaker != nil, @"%@.make should only be used inside a call to buildSubviews.", @ metamacro_stringify(_DRYUI_VIEW));
    [self setTranslatesAutoresizingMaskIntoConstraints:NO];
    return self._dryuiConstraintMaker;
}

- (NSMutableArray *)dryuiStyles {
    NSMutableArray *retVal = objc_getAssociatedObject(self, &dryui_stylesId);
    if (!retVal) {
        retVal = [NSMutableArray array];
        objc_setAssociatedObject(self, &dryui_stylesId, retVal, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    }
    return retVal;
}

- (MASConstraintMaker *)_dryuiConstraintMaker {
    return objc_getAssociatedObject(self, &dryui_constraintMakerId);
}
- (void)set_dryuiConstraintMaker:(MASConstraintMaker *)_dryuiConstraintMaker {
    objc_setAssociatedObject(self, &dryui_constraintMakerId, _dryuiConstraintMaker, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

@end


////////////////////////////////////////////////////////////////////////////////////////////////////
id _dryui_instantiate_from_encoding(char *encoding) {
    NSString *encodingString = [NSString stringWithUTF8String:encoding];
    
    NSRange braceRange = [encodingString rangeOfString:@"{"];
    NSRange equalsRange = [encodingString rangeOfString:@"="];
    NSString *className = [encodingString substringWithRange:NSMakeRange(braceRange.location+braceRange.length, equalsRange.location-1)];
    
    id instance = [NSClassFromString(className) new];
    
    return instance;
}
