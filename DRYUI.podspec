Pod::Spec.new do |s|
  s.name             = "DRYUI"
  s.version          = "1.2.0"
  s.summary          = "A tiny library that provides a better API for building and styling view hierarchies in code."
  s.description      = %{
    DRYUI is a tiny library that provides a better API for building and styling view hierarchies in code.
    DRYUI uses [Masonry](https://github.com/Masonry/Masonry) for Autolayout.
  }
  s.homepage         = "https://github.com/GriffinSchneider/DRYUI"
  s.license          = 'MIT'
  s.author           = { "Griffin Schneider" => "griffinschneider@gmail.com" }
  s.source           = { :git => "https://github.com/GriffinSchneider/DRYUI.git", :tag => s.version.to_s }
    
  s.ios.frameworks = 'Foundation', 'UIKit'

  s.requires_arc = true
  
  s.ios.deployment_target = '8.0'
  s.osx.deployment_target = '10.11'

  s.subspec 'objc' do |objc|
    objc.ios.deployment_target = '7.0'
    objc.dependency 'DRYUI/base'
  end

  s.subspec 'base' do |base|
    base.source_files = 'DRYUI/' + '*.{h,m}'
    base.dependency 'Masonry'
  end

  s.subspec 'swift' do |swift|
    swift.source_files ='DRYUI/' + '*.{swift}'
    swift.ios.deployment_target = '8.0'
    swift.osx.deployment_target = '10.11'
    swift.dependency 'SnapKit', '~> 4.0'
    swift.dependency 'DRYUI/base'
  end
  
end
