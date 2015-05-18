# DRYUI [![Version](https://img.shields.io/cocoapods/v/DRYUI.svg?style=flat)](http://cocoapods.org/pods/DRYUI) [![Build Status](https://travis-ci.org/GriffinSchneider/DRYUI.svg?branch=fix-travis)](https://travis-ci.org/GriffinSchneider/DRYUI)

DRYUI is a tiny library that provides a better API for building and styling view hierarchies in code. DRYUI uses [Masonry](https://github.com/Masonry/Masonry) for Autolayout, and works on both iOS and OSX.


## Installation

DRYUI is available through [CocoaPods](http://cocoapods.org). To install
it, just add the following line to your Podfile:

```ruby
pod DRYUI
```


##Hierarchy Building

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
#### Building hierarchies with DRYUI
```obj-c
build_subviews(self.view) {
    UIView *add_subview(view1) {
        _.backgroundColor = [UIColor redColor];
        _.make.edges.equalTo(superview);
        UIView *add_subview(view2) {
            _.backgroundColor = [UIColor blueColor];
            _.make.edges.equalTo(superview);
        };
    };
};
```
####Advantages:
- The hierarchy of the code mirrors the hierarchy of the views it creates - nested blocks correspond directly to nested subviews.
- Less repetitive boilerplate - no more forgetting to add your view as a subview.
- Less opportunity for copy-paste errors - since `_` and `superview` are always bound to the current view, moving lines from one block to another automatically causes them to affect the view whose block they're moved to.
- Views are initialized for you - if you declare your subview as `UIView`, DRYUI will `[[UIView alloc] init]` it automagically.


##Styling

####Basics
You can define a style like this in a `.m` file:
```obj-c
dryui_style(RedView) {
    _.backgroundColor = [UIColor redColor];
};
```
now you have a style named `RedView` that can be applied to views in `add_subview` like this:
```obj-c
build_subviews(self.view) {
    UIView *add_subview(thisViewIsRed, RedView) {
        // thisViewIsRed is now red.
    };
};
```
You can add any number of styles to a view in this way, and they'll be applied by calling the style blocks in the order they were added. The main block after `add_subview` runs after all styles have been applied, so it can override them.

If you want your style to be public, add a declaration like this to the `.h` file:
```obj-c
dryui_public_style(RedView);
```
This just `extern`s the style for you so you can use it globally.

####Styling for specific classes
By default, style blocks assume they're being applied to `UIViews`. If you want to style a subclass of `UIView`, just add the class name as the second argument to `dryui_style`:
```obj-c
dryui_style(RedButton, UIButton) {
    // _ is now a UIButton *
};
```
Attempting to apply a style like this to a view of the wrong type will trigger an exception at runtime.

####Style inheritance
Styles also support simple inheritance like this:
```obj-c
dryui_style(RedView) {
    _.backgroundColor = [UIColor redColor];
};
dryui_style(RedAndWhiteLabel, UILabel) {
    parent_style(RedView);
    _.textColor = [UIColor whiteColor];
};
// Now, applying the RedAndWhiteLabel style to a UILabel will give it a red background and white text.
```


##Advanced Usage of `add_subview`:

`add_subview` takes as its first argument a variable name. If a variable of that name has already been declared, `add_subview` will initialize a new object of that variable's type and assign it to the variable. If it hasn't been declared, put the type you want the variable to have before `add_subview` and `add_subview` will declare the variable and assign to it. You can also have `add_subview` assign to a property by passing in `self.propertyName`.

`add_subview` can optionally take an instance of `UIView` as its second argument. If you call `add_subview` this way, then that instance will be assigned to the variable instead of the defualt `[[<variable type> alloc] init]` that happens otherwise.

After the first (and optional second) argument, a list of `DRYUIStyle`s can be passed to `add_subview` to apply all the given styles to the view.

Here are some examples:
```obj-c
__block UIButton *secondButton, *thirdButton;
build_subviews(self) {
    UIButton *add_subview(firstButton) {};
    // The above line is equivalent to:
    //     UIButton *firstButton = [[UIButton alloc] init]; 
    //     [_ addSubview:firstButton];
    
    add_subview(secondButton) {};
    // secondButton has already been declared, to this is equivalent to:
    //     secondButton = [[UIButton alloc] init];
    //     [_ addSubview:secondButton];
    
    thirdButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
    add_subview(thirdButton) {};
    // thirdButton has already been assigned, so this just does:
    //    [_ addSubview:thirdButton];
    
    UIButton *add_subview(fourthButton, [UIButton buttonWithType:UIButtonTypeSystem]) {};
    // The second argument to add_subview is a view instance, so we'll use it for assignment
    // instead of the default alloc init:
    //     UIButton *fourthButton = [UIButton buttonWithType:UIButtonTypeSystem];
    //     [_ addSubview:fourthButton];
    
    UIButton *add_subview(fifthButton, RedView, RedButton) {};
    // Simple style application - this will create a new button named fifthButton and
    // apply the RedView and RedButton styles to it.
    
    UIButton *add_subview(sixthButton, [UIButton buttonWithType:UIButtonTypeSystem], RedView, RedButton) {};
    // Style application with a custom view instance - add_subview can detect that its second argument
    // is a custom view instance instead of a style, so this will do:
    //     UIButton *sixthButton = [UIButton buttonWithType:UIButtonTypeSystem];
    //     [_ addSubview:sixthButton];
    // and then add the RedView and RedButton styles to sixthButton.
};
```

Each call to `add_subview` will add the given view  as a subview to the view passed to the most
recent nested call of `build_subviews` or `add_subview`. The order of views will be determined by
the order of their `add_subview` calls.

Calls to `add_subview` must be inside a call to `build_subviews`. `build_subviews` won't add the given view as a subview to anything, since it's not nested within another call.

Within the block after the `add_subview` macro, `_` refers to the given view, and `superview`
refers to that view's superview. `_.make` is set to a `MASConstraintMaker` for the given view, similar
to the block passed to [Masonry](https://github.com/Masonry/Masonry)'s `mas_makeConstraints:` method.

The after-`add_subview`-block will only be called once everything else in the enclosing block has run, so
all views assigned at the same level of the hierarchy can reference one another. For example:
```obj-c
build_subviews(self) {
    __block UIView *one, *two;
    add_subview(one) {
        // Even though two is assigned below, it will not be nil here!
        _.make.left.equalTo(two);
    };
    add_subview(two) {
        // one won't be nil here either.
        _.make.top.equalTo(one);
    };
};
```
