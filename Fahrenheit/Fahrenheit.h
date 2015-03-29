//
//  Fahrenheit.h
//
//  Created by Griffin Schneider on 3/3/15.
//

/*!
 * First, an example. Assuming self is a UIView, this code will set self's background
 * to red, and add a green subview and a blue subview to self:
 *
 *   [self buildSubviews:º() {
 *       _.backgroundColor = [UIColor redColor];
 *       UIView *blueView = add(º(UIView) {
 *           _.backgroundColor = [UIColor blueColor];
 *       });
 *       UIView *greenView = add(º([UIView new]) {
 *           _.backgroundColor = [UIColor greenColor];
 *       });
 *   });
 *
 * Each call to 'add' will add the given view  as a subview to the view given to the most
 * recent nested call of buildSubviews or add. The order of views will be determined by
 * the order of their add calls.
 *
 * Within the block after the º macro, '_' refers to the given view, and 'superview'
 * refers to that view's superview. 'add' is a block that will add another subview to
 * the given view. '_.make' is set to a MASConstraintMaker for the given view, similar
 * to the block passed to Masonry's makeConstraints:.
 *
 * The after-º-block will only be called once everything else in the enclosing block has run, so
 * all views assigned at the same level of the hierarchy can reference one another. For example:
 *
 * [self buildSubviews:º() {
 *     UIView *two;
 *     UIView *one = add(º(UIView) {
 *         // Even though two is assigned below, it will not be nil here!
 *         _.make.left.equalTo(two);
 *     });
 *     two = add(º(UIView) {
 *         // one won't be nil here either.
 *         _.make.top.equalTo(one);
 *     });
 * }];
 *
 * In some situations, 'typeof' can't determine the type of the first argument, or gets it too general
 * (i.e. 'id' when you know it's more specific). If this happens, you can insert the type you know as
 * an argument before the block, like this:
 *
 *   [self buildSubviews:º() {
 *       add(º([UIButton buttonWithType:UIButtonTypeCustom], UIButton*) {
 *           <... '_' now has type 'UIButton *' ...>
 *       });
 *   }];
 *
 * The goals of this crazyness are
 *   - To build a view hierarchy with code where the code's nesting mirrors the nesting of the created
 *     views, with matching indentation that makes the hierarchy clear.
 *   - To have full autocompletion and type safety when creating the hierarchy and setting up
 *     the views.
 *   - To keep full debuggability while doing all this (having too much code inside macro arguments
 *     hurts debuggability due to newline removal when macros are expanded).
 */

#import <Foundation/Foundation.h>
#import <Masonry/Masonry.h>

#if TARGET_OS_IPHONE

#import <UIKit/UIKit.h>
#define FAHRENHEIT_VIEW UIView

#elif TARGET_OS_MAC

#import <AppKit/AppKit.h>
#define FAHRENHEIT_VIEW_NAME NSView

#endif

#ifndef FAHRENHEIT_VIEW_NAME
#define FAHRENHEIT_VIEW_NAME _
#endif

#ifndef FAHRENHEIT_CUSTOM_ARGS_MACRO
#define º(args...) FAHRENHEIT(args)
#endif

/*! Type of the block that gets passed as an argument to add */
typedef void (^FahrenheitAddArgumentBlock)(id FAHRENHEIT_VIEW_NAME, FAHRENHEIT_VIEW *superview);
/*! Type of the block that's bound to 'add' inside the block passed to buildSubviews */
typedef id (^FahrenheitAddBlock)(FAHRENHEIT_VIEW *view, FAHRENHEIT_VIEW *superview, FahrenheitAddArgumentBlock block);
/*! Type of the block that's passed to buildSubviews */
typedef void (^FahrenheitBuildSubviewsBlock)(FAHRENHEIT_VIEW *FAHRENHEIT_VIEW_NAME, FahrenheitAddBlock add);


////////////////////////////////////////////////////////////////////////////////////////////////////
@interface FAHRENHEIT_VIEW (Fahrenheit)

@property (nonatomic, strong, readonly) MASConstraintMaker *make;

- (void)buildSubviews:(FahrenheitBuildSubviewsBlock)block;

@end


////////////////////////////////////////////////////////////////////////////////////////////////////
@interface NSObject (Fahrenheit)
+ (instancetype)fahrenheit_selfOrInstanceOfSelf;
- (instancetype)fahrenheit_selfOrInstanceOfSelf;
@end


#define FAHRENHEIT(args...) __FAHRENHEIT_HELPER(BOGUS, ## args)
#define __FAHRENHEIT_OVERLOADED_MACRO(_1,_2,_3,NAME,...) NAME
#define __FAHRENHEIT_HELPER(args...) __FAHRENHEIT_OVERLOADED_MACRO(args, __FAHRENHEIT_HELPER_3, __FAHRENHEIT_HELPER_2, __FAHRENHEIT_HELPER_1)(args)
#define __FAHRENHEIT_HELPER_1(BOGUS) ^(FAHRENHEIT_VIEW *FAHRENHEIT_VIEW_NAME, FahrenheitAddBlock add)
#define __FAHRENHEIT_HELPER_2(BOGUS, viewArg) __FAHRENHEIT_HELPER_3(BOGUS, viewArg, typeof([viewArg fahrenheit_selfOrInstanceOfSelf]))
#define __FAHRENHEIT_HELPER_3(BOGUS, viewArg, viewArgType) [viewArg fahrenheit_selfOrInstanceOfSelf], FAHRENHEIT_VIEW_NAME, ^(viewArgType FAHRENHEIT_VIEW_NAME, FAHRENHEIT_VIEW *superview)


