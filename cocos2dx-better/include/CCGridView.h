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
#ifndef __CCGridView__
#define __CCGridView__

#include "cocos-ext.h"
#include "CCTableViewCell.h"
#include "CCSorting.h"
#include <set>
#include <vector>
#include "CCMoreMacros.h"

USING_NS_CC_EXT;
using namespace std;

NS_CC_BEGIN

class CCGridView;

/**
 * Sole purpose of this delegate is to single touch event in this version.
 * @js NA
 * @lua NA
 */
class CC_DLL CCGridViewDelegate : public CCScrollViewDelegate
{
public:
    /**
     * Delegate to respond touch event
     *
     * @param table table contains the given cell
     * @param cell  cell that is touched
     */
    virtual void tableCellTouched(CCGridView* table, CCTableViewCell* cell) = 0;
    
    /**
     * Delegate to respond a table cell press event.
     *
     * @param table table contains the given cell
     * @param cell  cell that is pressed
     */
    virtual void tableCellHighlight(CCGridView* table, CCTableViewCell* cell){};
    
    /**
     * Delegate to respond a table cell release event
     *
     * @param table table contains the given cell
     * @param cell  cell that is pressed
     */
    virtual void tableCellUnhighlight(CCGridView* table, CCTableViewCell* cell){};
    
    /**
     * Delegate called when the cell is about to be recycled. Immediately
     * after this call the cell will be removed from the scene graph and
     * recycled.
     *
     * @param table table contains the given cell
     * @param cell  cell that is pressed
     */
    virtual void tableCellWillRecycle(CCGridView* table, CCTableViewCell* cell){};
};

/**
 * Data source that governs table backend data.
 * @lua NA
 */
class CC_DLL CCGridViewDataSource
{
public:
    virtual ~CCGridViewDataSource() {}
    
    /**
     * cell size for a given index
     *
     * @param idx the index of a cell to get a size
     * @return size of a cell at given index
     */
    virtual CCSize tableCellSizeForIndex(CCGridView *table, unsigned int idx) = 0;
    
    /**
     * a cell instance at a given index
     *
     * @param idx index to search for a cell
     * @return cell found at idx
     */
    virtual CCTableViewCell* tableCellAtIndex(CCGridView *table, unsigned int idx) = 0;
    
    /**
     * Returns number of cells in a given table view.
     *
     * @return number of cells
     */
    virtual unsigned int numberOfCellsInTableView(CCGridView *table) = 0;
    
};


/**
 * The code is copied from CCTableView for fix following:
 * 1. change some method to virtual so that subclass can override it, such as CCGridView
 */
class CC_DLL CCGridView : public CCScrollView, public CCScrollViewDelegate
{
public:
    /**
     *  @js ctor
     */
    CCGridView();
    /**
     *  @js NA
     */
    virtual ~CCGridView();
    
    /**
     * An intialized table view object
     *
     * @param dataSource data source
     * @param size view size
     * @return table view
     */
    static CCGridView* create(CCGridViewDataSource* dataSource, CCSize size);
    /**
     * An initialized table view object
     *
     * @param dataSource data source;
     * @param size view size
     * @param container parent object for cells
     * @return table view
     */
    static CCGridView* create(CCGridViewDataSource* dataSource, CCSize size, CCNode *container);
    
    /**
     * data source
     * @js NA
     */
    CCGridViewDataSource* getDataSource() { return m_pDataSource; }
    void setDataSource(CCGridViewDataSource* source) { m_pDataSource = source; }
    /**
     * delegate
     * @js NA
     */
    CCGridViewDelegate* getDelegate() { return m_pTableViewDelegate; }
    void setDelegate(CCGridViewDelegate* pDelegate) { m_pTableViewDelegate = pDelegate; }
    
    
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
    
    int getRealRows();
    CCPoint getTouchPoint() { return m_tTouchPoint; }
    
protected:
    
    CCTableViewCell *m_pTouchedCell;
    
    /**
     * index set to query the indexes of the cells used.
     */
    set<unsigned int>* m_pIndices;
    
    vector<float> m_vCellsPositions; // relative to top
    vector<float> m_hCellsPositions;
    int m_viewRows; // for horizontal
    
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
    CCGridViewDataSource* m_pDataSource;
    /**
     * weak link to the delegate object
     */
    CCGridViewDelegate* m_pTableViewDelegate;
    
	CCScrollViewDirection m_eOldDirection;
    
    virtual int _indexFromOffset(CCPoint offset);
    virtual CCPoint _offsetFromIndex(unsigned int index);
    
    void _moveCellOutOfSight(CCTableViewCell *cell);
    void _setIndexForCell(unsigned int index, CCTableViewCell *cell);
    void _addCellIfNecessary(CCTableViewCell * cell);
    void _updateCellPositions();
    
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
    
    /// column count, by default it is 1
    CC_SYNTHESIZE_SETTER(unsigned int, m_colCount, ColCount);
};

NS_CC_END

#endif /* __CCGridView__ */
