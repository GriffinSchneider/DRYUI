Pod::Spec.new do |s|
  s.name             = "DRYUI"
  s.version          = "0.9.0-alpha-4"
  s.summary          = "A tiny library that provides a better API for building and styling view hierarchies in code."
  s.description      = %{
    DRYUI is a tiny library that provides a better API for building and styling view hierarchies in code.
    DRYUI uses [Masonry](https://github.com/Masonry/Masonry) for Autolayout.
  }
  s.homepage         = "https://github.com/GriffinSchneider/DRYUI"
  s.license          = 'MIT'
  s.author           = { "Griffin Schneider" => "griffinschneider@gmail.com" }
  s.source           = { :git => "https://github.com/GriffinSchneider/DRYUI.git", :tag => s.version.to_s }
  
  s.source_files = 'DRYUI/**'
  
  s.ios.frameworks = 'Foundation', 'UIKit'

  s.ios.deployment_target = '8.0'
  s.requires_arc = true

  s.dependency 'Masonry'
  s.dependency 'SnapKit'
end
