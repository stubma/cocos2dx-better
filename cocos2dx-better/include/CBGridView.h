/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-better
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#ifndef __CBGridView__
#define __CBGridView__

#include "cocos2d.h"
#include "CBTableView.h"
#include "CCMoreMacros.h"

NS_CC_BEGIN

/**
 * It adds multiple column support to CBTableView.
 * Use CCScrollViewDirectionBoth to set custom column count, colCount.
 * If a specific direction is set, table automatically find a proper value,
 * computing from viewSize and cellSize.
 */
class CBGridView : public CBTableView {
protected:
    // override super
    virtual void _updateCellPositions();
    virtual void _updateContentSize();
    virtual int __indexFromOffset(CCPoint offset);
    virtual CCPoint __offsetFromIndex(unsigned int index);
    
public:
    CBGridView();
       
    /**
     * An intialized table view object
     *
     * @param dataSource data source
     * @param size view size
     * @return table view
     */
    static CBGridView* create(CBTableViewDataSource* dataSource, CCSize size);
    
    /**
     * An initialized table view object
     *
     * @param dataSource data source;
     * @param size view size
     * @param container parent object for cells
     * @return table view
     */
    static CBGridView* create(CBTableViewDataSource* dataSource, CCSize size, CCNode *container);
    
    /// column count, by default it is 1
    CC_SYNTHESIZE_SETTER(unsigned int, m_colCount, ColCount);
};

NS_CC_END

#endif /* __CBGridView__ */
