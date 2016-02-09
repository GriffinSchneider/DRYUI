import SnapKit

public extension UIView {
    
    public func addSubview<SubviewType:UIView>(subviewBlocks: (SubviewType) -> Void ...) -> SubviewType {
        return addSubviewInternal(SubviewType(), subviewBlocks)
    }
    
    public func addSubview<SubviewType:UIView>(subviewBlocks: (SubviewType) -> Void ..., _ lastBlock: (SubviewType, ConstraintMaker) -> Void) -> SubviewType {
        return addSubviewInternal(SubviewType(), subviewBlocks, lastBlock)
    }
    
    public func addSubview<SubviewType:UIView>(subview: SubviewType, _ subviewBlocks: (SubviewType) -> Void ...) -> SubviewType {
        return addSubviewInternal(subview, subviewBlocks)
    }
    
    public func addSubview<SubviewType:UIView>(subview: SubviewType, _ subviewBlocks: (SubviewType) -> Void ..., _ lastBlock: (SubviewType, ConstraintMaker) -> Void) -> SubviewType {
        return addSubviewInternal(subview, subviewBlocks, lastBlock)
    }
    
    // These 'internal' methods _should_ be just more overloads of addSubview, but doing it that way makes the compiler crash (XCode 7.2).
    
    private func addSubviewInternal<SubviewType:UIView>(subview: SubviewType, _ subviewBlocks: [(SubviewType) -> Void]) -> SubviewType {
        self.addSubview(subview)
        subviewBlocks.forEach{ $0(subview) }
        return subview
    }
    
    private func addSubviewInternal<SubviewType:UIView>(subview: SubviewType, _ subviewBlocks: [(SubviewType) -> Void], _ lastBlock: (SubviewType, ConstraintMaker) -> Void) -> SubviewType {
        self.addSubview(subview)
        subviewBlocks.forEach{ $0(subview) }
        snp_makeConstraints { lastBlock(subview, $0) }
        return subview
    }
}
