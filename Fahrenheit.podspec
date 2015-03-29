#
# Be sure to run `pod lib lint Fahrenheit.podspec' to ensure this is a
# valid spec and remove all comments before submitting the spec.
#
# Any lines starting with a # are optional, but encouraged
#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = "Fahrenheit"
  s.version          = "0.1.0"
  s.summary          = "A tiny library that provides a better API for building view hierarchies in code."
  s.description      = %{
    Fahrenheit is a tiny library that provides a better API for building view hierarchies in code.
    Fahrenheit uses [Masonry](https://github.com/Masonry/Masonry) for Autolayout, and works on both iOS and OSX.
  }
  s.homepage         = "https://github.com/GriffinSchneider/Fahrenheit"
  s.license          = 'MIT'
  s.author           = { "Griffin Schneider" => "griffinschneider@gmail.com" }
  s.source           = { :git => "https://github.com/GriffinSchneider/Fahrenheit.git", :tag => s.version.to_s }
  
  s.source_files = 'Fahrenheit/**'
  
  s.ios.frameworks = 'Foundation', 'UIKit'
  s.osx.frameworks = 'Foundation', 'AppKit'

  s.ios.deployment_target = '6.0'
  s.osx.deployment_target = '10.7'
  s.requires_arc = true

  s.dependency 'Masonry'
end
