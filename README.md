# Fahrenheit [![Version](https://img.shields.io/cocoapods/v/Fahrenheit.svg?style=flat)](http://cocoapods.org/pods/Fahrenheit)[![Build Status](https://travis-ci.org/GriffinSchneider/Fahrenheit.svg?branch=fix-travis)](https://travis-ci.org/GriffinSchneider/Fahrenheit)

Fahrenheit is a tiny library that provides a better API for building view hierarchies in code. Fahrenheit uses [Masonry](https://github.com/Masonry/Masonry) for Autolayout, and works on both iOS and OSX.

To demonstrate, let's construct a simple view hierarchy, with 2 views nested inside `self.view`. 

#### Building hierarchies normally
```obj-c
UIView *view1 = [UIView new];
view1.backgroundColor = [UIColor redColor];
[self.view addSubview:view1]
[view1 mas_makeConstraints:^(MASConstraintMaker *make) {
    make.edges.equalTo(view1.superview);
}];

UIView *view2 = [UIView new];
view2.backgroundColor = [UIColor blueColor];
[view1 addSubview:view2];
[view2 mas_makeConstraints:^(MASConstraintMaker *make) {
    make.edges.equalTo(view1);
}];
```
#### Building hierarchies with Fahrenheit
```obj-c
UIView *view1, *view2;
[self.view buildSubviews:º() {
    view1 = add(º(UIView) {
        _.backgroundColor = [UIColor redColor];
        _.make.edges.equalTo(superview);
        view2 = add(º(UIView) {
            _.backgroundColor = [UIColor blueColor];
            _.make.edges.equalTo(superview);
        });
    });
}];
```
_(You can type `º` on OSX by pressing `⌥-0`)_
####Advantages:
- The hierarchy of the code mirrors the hierarchy of the views it creates - nested blocks correspond directly to nested subviews.
- Less repetitive boilerplate - no more forgetting to add your view as a subview.
- Less opportunity for copy-paste errors - since `_` and `superview` are always bound to the current view, moving lines from one block to another automatically causes them to affect the view whose block they're moved to.

####Advanced Usage:
Each call to `add` will add the given view  as a subview to the view passed to the most
recent nested call of `buildSubviews` or `add`. The order of views will be determined by
the order of their `add` calls.

Within the block after the `º` macro, `_`refers to the given view, and `superview`
refers to that view's superview. `add` is a block that will add another subview to
the given view. `_.make` is set to a `MASConstraintMaker` for the given view, similar
to the block passed to [Masonry](https://github.com/Masonry/Masonry)'s `mas_makeConstraints:` method.

The after-`º`-block will only be called once everything else in the enclosing block has run, so
all views assigned at the same level of the hierarchy can reference one another. For example:
```obj-c
[self buildSubviews:º() {
    UIView *one, *two;
    one = add(º(UIView) {
        // Even though two is assigned below, it will not be nil here!
        _.make.left.equalTo(two);
    });
    two = add(º(UIView) {
        // one won't be nil here either.
        _.make.top.equalTo(one);
    });
}];
```


When you pass an instance as the first argument to `º`, `typeof` is used to determine the instance's
type. In some situations, `typeof` can't determine the type, or comes up with a type that's too generic
(i.e. `id` when you know it's more specific). If this happens, you can cast the instance yourself:
```obj-c 
[self buildSubviews:º() {
    add(º((UIButton *)[UIButton buttonWithType:UIButtonTypeCustom]) {
        <... _ now has type UIButton*, even though buttonWithType: returns id ...>
    });
}];
```

## Installation

Fahrenheit is available through [CocoaPods](http://cocoapods.org). To install
it, just add the following line to your Podfile:

```ruby
pod 'Fahrenheit'
```

If you don't like the `º` symbol (or if you've found an even cooler use for it), you can configure Fahrenheit to use whatever you want instead. Add these 2 lines to your prefix.pch before importing Fahrenheit:
```obj-c
#define FAHRENHEIT_CUSTOM_ARGS_MACRO
#define f(args...) FAHRENHEIT(args)
```
...and now you can use `f`, or anything you replace `f` with, in place of `º`.
