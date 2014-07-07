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
#ifndef __CBTableView__
#define __CBTableView__

#include "cocos-ext.h"
#include "CCTableViewCell.h"
#include "CCSorting.h"
#include <set>
#include <vector>

USING_NS_CC_EXT;

NS_CC_BEGIN

class CBTableView;

/**
 * Sole purpose of this delegate is to single touch event in this version.
 * @js NA
 * @lua NA
 */
class CC_DLL CBTableViewDelegate : public CCScrollViewDelegate
{
public:
    /**
     * Delegate to respond touch event
     *
     * @param table table contains the given cell
     * @param cell  cell that is touched
     */
    virtual void tableCellTouched(CBTableView* table, CCTableViewCell* cell) = 0;
    
    /**
     * Delegate to respond a table cell press event.
     *
     * @param table table contains the given cell
     * @param cell  cell that is pressed
     */
    virtual void tableCellHighlight(CBTableView* table, CCTableViewCell* cell){};
    
    /**
     * Delegate to respond a table cell release event
     *
     * @param table table contains the given cell
     * @param cell  cell that is pressed
     */
    virtual void tableCellUnhighlight(CBTableView* table, CCTableViewCell* cell){};
    
    /**
     * Delegate called when the cell is about to be recycled. Immediately
     * after this call the cell will be removed from the scene graph and
     * recycled.
     *
     * @param table table contains the given cell
     * @param cell  cell that is pressed
     */
    virtual void tableCellWillRecycle(CBTableView* table, CCTableViewCell* cell){};
    
};


/**
 * Data source that governs table backend data.
 * @lua NA
 */
class CC_DLL CBTableViewDataSource
{
public:
    virtual ~CBTableViewDataSource() {}
    
    /**
     * cell size for a given index
     *
     * @param idx the index of a cell to get a size
     * @return size of a cell at given index
     */
    virtual CCSize tableCellSizeForIndex(CBTableView *table, unsigned int idx) {
        return cellSizeForTable(table);
    };
    /**
     * cell height for a given table.
     *
     * @param table table to hold the instances of Class
     * @return cell size
     */
    virtual CCSize cellSizeForTable(CBTableView *table) {
        return CCSizeZero;
    };
    /**
     * a cell instance at a given index
     *
     * @param idx index to search for a cell
     * @return cell found at idx
     */
    virtual CCTableViewCell* tableCellAtIndex(CBTableView *table, unsigned int idx) = 0;
    /**
     * Returns number of cells in a given table view.
     *
     * @return number of cells
     */
    virtual unsigned int numberOfCellsInTableView(CBTableView *table) = 0;
    
};


/**
 * The code is copied from CCTableView for fix following:
 * 1. change some method to virtual so that subclass can override it, such as CBGridView
 */
class CC_DLL CBTableView : public CCScrollView, public CCScrollViewDelegate
{
public:
    /**
     *  @js ctor
     */
    CBTableView();
    /**
     *  @js NA
     */
    virtual ~CBTableView();
    
    /**
     * An intialized table view object
     *
     * @param dataSource data source
     * @param size view size
     * @return table view
     */
    static CBTableView* create(CBTableViewDataSource* dataSource, CCSize size);
    /**
     * An initialized table view object
     *
     * @param dataSource data source;
     * @param size view size
     * @param container parent object for cells
     * @return table view
     */
    static CBTableView* create(CBTableViewDataSource* dataSource, CCSize size, CCNode *container);
    
    /**
     * data source
     * @js NA
     */
    CBTableViewDataSource* getDataSource() { return m_pDataSource; }
    void setDataSource(CBTableViewDataSource* source) { m_pDataSource = source; }
    /**
     * delegate
     * @js NA
     */
    CBTableViewDelegate* getDelegate() { return m_pTableViewDelegate; }
    void setDelegate(CBTableViewDelegate* pDelegate) { m_pTableViewDelegate = pDelegate; }
    
    /**
     * determines how cell is ordered and filled in the view.
     */
    void setVerticalFillOrder(CCTableViewVerticalFillOrder order);
    CCTableViewVerticalFillOrder getVerticalFillOrder();
    
    
    bool initWithViewSize(CCSize size, CCNode* container = NULL);
    /**
     * Updates the content of the cell at a given index.
     *
     * @param idx index to find a cell
     */
    void updateCellAtIndex(unsigned int idx);
    /**
     * Inserts a new cell at a given index
     *
     * @param idx location to insert
     */
    void insertCellAtIndex(unsigned int idx);
    /**
     * Removes a cell at a given index
     *
     * @param idx index to find a cell
     */
    void removeCellAtIndex(unsigned int idx);
    /**
     * reloads data from data source.  the view will be refreshed.
     */
    void reloadData();
    /**
     * Dequeues a free cell if available. nil if not.
     *
     * @return free cell
     */
    CCTableViewCell *dequeueCell();
    
    /**
     * Returns an existing cell at a given index. Returns nil if a cell is nonexistent at the moment of query.
     *
     * @param idx index
     * @return a cell at a given index
     */
    CCTableViewCell *cellAtIndex(unsigned int idx);
    
    
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view) {}
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
    
    CCTableViewCell *m_pTouchedCell;
    /**
     * vertical direction of cell filling
     */
    CCTableViewVerticalFillOrder m_eVordering;
    
    /**
     * index set to query the indexes of the cells used.
     */
    std::set<unsigned int>* m_pIndices;
    
    /**
     * vector with all cell positions
     */
    std::vector<float> m_vCellsPositions;
    //NSMutableIndexSet *indices_;
    /**
     * cells that are currently in the table
     */
    CCArrayForObjectSorting* m_pCellsUsed;
    /**
     * free list of cells
     */
    CCArrayForObjectSorting* m_pCellsFreed;
    /**
     * weak link to the data source object
     */
    CBTableViewDataSource* m_pDataSource;
    /**
     * weak link to the delegate object
     */
    CBTableViewDelegate* m_pTableViewDelegate;
    
	CCScrollViewDirection m_eOldDirection;
    
    virtual int __indexFromOffset(CCPoint offset);
    unsigned int _indexFromOffset(CCPoint offset);
    virtual CCPoint __offsetFromIndex(unsigned int index);
    CCPoint _offsetFromIndex(unsigned int index);
    
    void _moveCellOutOfSight(CCTableViewCell *cell);
    void _setIndexForCell(unsigned int index, CCTableViewCell *cell);
    void _addCellIfNecessary(CCTableViewCell * cell);
    
    virtual void _updateCellPositions();
public:
    virtual void _updateContentSize();
    
    enum TableViewScriptEventType
    {
        kTableViewScroll   = 0,
        kTableViewZoom,
        kTableCellTouched,
        kTableCellHighLight,
        kTableCellUnhighLight,
        kTableCellWillRecycle,
        kTableCellSizeForIndex,
        kTableCellSizeAtIndex,
        kNumberOfCellsInTableView,
    };
    void unregisterAllScriptHandler();
};

NS_CC_END

#endif /* __CBTableView__ */
