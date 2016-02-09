//
//  SwiftViewController.swift
//  DRYUI Example
//
//  Created by Griffin on 2/9/16.
//  Copyright © 2016 Griffin Schneider. All rights reserved.
//

import Foundation
import DRYUI

@objc class SwiftStyles: NSObject {
    static func aStyle(title: String) -> (v: UILabel) -> Void {
        return {v in v.text = title}
    }
}

extension UIView {
    
    func addSubview<SubviewType:UIView>(subviewBlocks: (SubviewType) -> Void ...) -> Void {
        addSubview(SubviewType(), subviewBlocks)
    }
    
    func addSubview<SubviewType:UIView>(subview: SubviewType, _ subviewBlocks: (SubviewType) -> Void ...) -> SubviewType {
        return addSubview(subview, subviewBlocks)
    }
    
    func addSubview<SubviewType: UIView>(subview: SubviewType, _ subviewBlocks:[(SubviewType) -> Void]) -> SubviewType {
        self.addSubview(subview)
        subviewBlocks.forEach{ $0(subview) }
        return subview
    }
    
}

class SwiftViewController: UIViewController {
    override func viewDidLoad() {
        self.view.addSubview(Green, Orange, {(v: UIButton) -> Void in
            v.frame = self.view.frame;
            v.addSubview(Green, SwiftStyles.aStyle("This is the title"), {(v: UILabel) in
                v.frame = self.view.frame;
            })
        })
    }
}