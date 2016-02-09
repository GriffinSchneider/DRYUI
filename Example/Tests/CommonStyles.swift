//
//  CommonStyles.swift
//  DRYUI Example
//
//  Created by Schneider, Griffin on 2/9/16.
//  Copyright © 2016 Griffin Schneider. All rights reserved.
//

import Foundation

@objc class CommonStyles: NSObject {
    static var Red: (UIView) -> Void {
        return { (v: UIView) in
            v.backgroundColor = UIColor.redColor()
        }
    }
}