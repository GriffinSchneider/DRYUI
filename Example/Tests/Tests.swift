//
//  Tests.swift
//  DRYUI Example
//
//  Created by Schneider, Griffin on 2/9/16.
//  Copyright © 2016 Griffin Schneider. All rights reserved.
//

import Foundation
import XCTest
import DRYUI
import SnapKit

class DRYUISwiftTests: XCTestCase {
    
    func testDRYUI() {
        let topView = UIView()
        
        let v1 = topView.addSubview(CommonStyles.Red)
        XCTAssertEqual(v1.backgroundColor, UIColor.red)
        XCTAssertEqual(v1.superview, topView)
        XCTAssertTrue(v1.translatesAutoresizingMaskIntoConstraints)
        
        let v2 = topView.addSubview(CommonStyles.Red, { v in
            XCTAssertEqual(v.superview, topView)
            v.backgroundColor = UIColor.green
        })
        XCTAssertEqual(v2.backgroundColor, UIColor.green)
        XCTAssertEqual(v2.superview, topView)
        XCTAssertTrue(v2.translatesAutoresizingMaskIntoConstraints)
        
        let v3 = topView.addSubview(Blue, CommonStyles.Red)
        XCTAssertEqual(v3.backgroundColor, UIColor.red)
        XCTAssertEqual(v3.superview, topView)
        XCTAssertTrue(v3.translatesAutoresizingMaskIntoConstraints)
        
        let v4 = topView.addSubview(Blue) {(v: UIView, make: ConstraintMaker) in
            make.edges.equalTo(v.superview!)
        }
        XCTAssertEqual(v4.backgroundColor, UIColor.blue)
        XCTAssertEqual(v4.superview, topView)
        XCTAssertFalse(v4.translatesAutoresizingMaskIntoConstraints)
        
    }
}
