//
//  SwiftViewController.swift
//  DRYUI Example
//
//  Created by Griffin on 2/9/16.
//  Copyright © 2016 Griffin Schneider. All rights reserved.
//

import Foundation
import DRYUI
import SnapKit

@objc class SwiftStyles: NSObject {
    static func aStyle(_ title: String) -> (_ v: UILabel) -> Void {
        return {v in v.text = title}
    }
}


class SwiftViewController: UIViewController {
    override func viewDidLoad() {
        self.view.addSubview(Green, Orange) {(v: UIButton, make: ConstraintMaker) -> Void in
            v.addSubview(Green, SwiftStyles.aStyle("asdf")) {(v: UILabel, make: ConstraintMaker) in
                make.edges.equalTo(v.superview!)
            }
        }
    }
}
