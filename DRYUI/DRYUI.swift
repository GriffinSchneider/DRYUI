import SnapKit

#if os(iOS) || os(tvOS)
    import UIKit
    public typealias View = UIView
#else
    import AppKit
    public typealias View = NSView
#endif

public protocol DRYUIAble {
    @discardableResult func addSubview<SubviewType:View>(_ subviewBlocks: (SubviewType) -> Void ...) -> SubviewType
    @discardableResult func addSubview<SubviewType:View>(_ subviewBlocks: (SubviewType) -> Void ..., _ lastBlock: (SubviewType, ConstraintMaker) -> Void) -> SubviewType
    @discardableResult func addSubview<SubviewType:View>(_ subviewType: SubviewType.Type, _ subviewBlocks: (SubviewType) -> Void ...) -> SubviewType
    @discardableResult func addSubview<SubviewType:View>(_ subviewType: SubviewType.Type, _ subviewBlocks: (SubviewType) -> Void ..., _ lastBlock: (SubviewType, ConstraintMaker) -> Void) -> SubviewType
    @discardableResult func addSubview<SubviewType:View>(_ subview: SubviewType, _ subviewBlocks: (SubviewType) -> Void ...) -> SubviewType
    @discardableResult func addSubview<SubviewType:View>(_ subview: SubviewType, _ subviewBlocks: (SubviewType) -> Void ..., _ lastBlock: (SubviewType, ConstraintMaker) -> Void) -> SubviewType
}

extension View: DRYUIAble {
    // MARK: Add a view with inferred type from blocks
    @discardableResult public func addSubview<SubviewType:View>(_ subviewBlocks: (SubviewType) -> Void ...) -> SubviewType {
        let subview = SubviewType()
        addSubview(subview)
        subviewBlocks.forEach{ $0(subview) }
        return subview
    }
    
    @discardableResult public func addSubview<SubviewType:View>(_ subviewBlocks: (SubviewType) -> Void ..., _ lastBlock: (SubviewType, ConstraintMaker) -> Void) -> SubviewType {
        let subview = SubviewType()
        addSubview(subview)
        subviewBlocks.forEach{ $0(subview) }
        subview.snp.makeConstraints { lastBlock(subview, $0) }
        return subview
    }
    
    
    // MARK: Add a view with type passed explicitly
    @discardableResult public func addSubview<SubviewType:View>(_ subviewType: SubviewType.Type, _ subviewBlocks: (SubviewType) -> Void ...) -> SubviewType {
        let subview = SubviewType()
        addSubview(subview)
        subviewBlocks.forEach{ $0(subview) }
        return subview
    }
    
    @discardableResult public func addSubview<SubviewType:View>(_ subviewType: SubviewType.Type, _ subviewBlocks: (SubviewType) -> Void ..., _ lastBlock: (SubviewType, ConstraintMaker) -> Void) -> SubviewType {
        let subview = SubviewType()
        addSubview(subview)
        subviewBlocks.forEach{ $0(subview) }
        subview.snp.makeConstraints { lastBlock(subview, $0) }
        return subview
    }
    
    
    // MARK: Add a view with the view instance passed explicitly
    @discardableResult public func addSubview<SubviewType:View>(_ subview: SubviewType, _ subviewBlocks: (SubviewType) -> Void ...) -> SubviewType {
        addSubview(subview)
        subviewBlocks.forEach{ $0(subview) }
        return subview
    }
    
    @discardableResult public func addSubview<SubviewType:View>(_ subview: SubviewType, _ subviewBlocks: (SubviewType) -> Void ..., _ lastBlock: (SubviewType, ConstraintMaker) -> Void) -> SubviewType {
        addSubview(subview)
        subviewBlocks.forEach{ $0(subview) }
        subview.snp.makeConstraints { lastBlock(subview, $0) }
        return subview
    }
}
