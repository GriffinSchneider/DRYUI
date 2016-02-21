import SnapKit

#if os(iOS) || os(tvOS)
    import UIKit
    public typealias View = UIView
#else
    import AppKit
    public typealias View = NSView
#endif

public protocol DRYUIAble {
    func dryui_addSubview(view: View) -> Void
}

extension View: DRYUIAble {
    public func dryui_addSubview(view: View) {
        addSubview(view)
    }
}

public extension DRYUIAble {
    // MARK: Add a view with inferred type from blocks
    public func addSubview<SubviewType:View>(subviewBlocks: (SubviewType) -> Void ...) -> SubviewType {
        return addSubviewInternal(SubviewType(), subviewBlocks)
    }
    
    public func addSubview<SubviewType:View>(subviewBlocks: (SubviewType) -> Void ..., @noescape _ lastBlock: (SubviewType, ConstraintMaker) -> Void) -> SubviewType {
        return addSubviewInternal(SubviewType(), subviewBlocks, lastBlock)
    }
    
    
    // MARK: Add a view with type passed explicitly
    public func addSubview<SubviewType:View>(subviewType: SubviewType.Type, _ subviewBlocks: (SubviewType) -> Void ...) -> SubviewType {
        return addSubviewInternal(SubviewType(), subviewBlocks)
    }
    
    public func addSubview<SubviewType:View>(subviewType: SubviewType.Type, _ subviewBlocks: (SubviewType) -> Void ..., @noescape _ lastBlock: (SubviewType, ConstraintMaker) -> Void) -> SubviewType {
        return addSubviewInternal(SubviewType(), subviewBlocks, lastBlock)
    }
    
    
    // MARK: Add a view with the view instance passed explicitly
    public func addSubview<SubviewType:View>(subview: SubviewType, _ subviewBlocks: (SubviewType) -> Void ...) -> SubviewType {
        return addSubviewInternal(subview, subviewBlocks)
    }
    
    public func addSubview<SubviewType:View>(subview: SubviewType, _ subviewBlocks: (SubviewType) -> Void ..., @noescape _ lastBlock: (SubviewType, ConstraintMaker) -> Void) -> SubviewType {
        return addSubviewInternal(subview, subviewBlocks, lastBlock)
    }
    
    
    // MARK: Private methods
    // These 'internal' methods _should_ be just more overloads of addSubview, but doing it that way makes the compiler crash (XCode 7.2).
    private func addSubviewInternal<SubviewType:View>(subview: SubviewType, _ subviewBlocks: [(SubviewType) -> Void]) -> SubviewType {
        self.dryui_addSubview(subview)
        subviewBlocks.forEach{ $0(subview) }
        return subview
    }
    
    private func addSubviewInternal<SubviewType:View>(subview: SubviewType, _ subviewBlocks: [(SubviewType) -> Void], @noescape _ lastBlock: (SubviewType, ConstraintMaker) -> Void) -> SubviewType {
        self.dryui_addSubview(subview)
        subviewBlocks.forEach{ $0(subview) }
        subview.snp_makeConstraints { lastBlock(subview, $0) }
        return subview
    }
}
