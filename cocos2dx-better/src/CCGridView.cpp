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
#include "CCGridView.h"
#include "CCTableViewCell.h"

NS_CC_BEGIN

CCGridView* CCGridView::create(CCGridViewDataSource* dataSource, CCSize size)
{
    return CCGridView::create(dataSource, size, NULL);
}

CCGridView* CCGridView::create(CCGridViewDataSource* dataSource, CCSize size, CCNode *container)
{
    CCGridView *table = new CCGridView();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setDataSource(dataSource);
    table->_updateCellPositions();
    table->_updateContentSize();
    
    return table;
}

bool CCGridView::initWithViewSize(CCSize size, CCNode* container/* = NULL*/)
{
    if (CCScrollView::initWithViewSize(size,container))
    {
        m_pCellsUsed = new CCArrayForObjectSorting();
        m_pCellsFreed = new CCArrayForObjectSorting();
        m_pIndices = new std::set<unsigned int>();
        m_colCount = 1;
        setDirection(kCCScrollViewDirectionVertical);
        
        CCScrollView::setDelegate(this);
        return true;
    }
    return false;
}

CCGridView::CCGridView()
: m_pTouchedCell(NULL)
, m_pIndices(NULL)
, m_pCellsUsed(NULL)
, m_pCellsFreed(NULL)
, m_pDataSource(NULL)
, m_pTableViewDelegate(NULL)
, m_eOldDirection(kCCScrollViewDirectionNone)
, m_viewRows(-1)
{
    
}

CCGridView::~CCGridView()
{
    CC_SAFE_DELETE(m_pIndices);
    CC_SAFE_RELEASE(m_pCellsUsed);
    CC_SAFE_RELEASE(m_pCellsFreed);
    unregisterAllScriptHandler();
}

void CCGridView::reloadData()
{
    m_eOldDirection = kCCScrollViewDirectionNone;
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(m_pCellsUsed, pObj)
    {
        CCTableViewCell* cell = (CCTableViewCell*)pObj;
        
        if(m_pTableViewDelegate != NULL) {
            m_pTableViewDelegate->tableCellWillRecycle(this, cell);
        }
        
        m_pCellsFreed->addObject(cell);
        cell->reset();
        if (cell->getParent() == getContainer())
        {
            getContainer()->removeChild(cell, true);
        }
    }
    
    m_pIndices->clear();
    m_pCellsUsed->release();
    m_pCellsUsed = new CCArrayForObjectSorting();
    
    _updateCellPositions();
    _updateContentSize();
    if (m_pDataSource->numberOfCellsInTableView(this) > 0)
    {
        scrollViewDidScroll(this);
    }
}

CCTableViewCell *CCGridView::cellAtIndex(unsigned int idx)
{
    CCTableViewCell *found = NULL;
    
    if (m_pIndices->find(idx) != m_pIndices->end())
    {
        found = (CCTableViewCell *)m_pCellsUsed->objectWithObjectID(idx);
    }
    
    return found;
}

void CCGridView::updateCellAtIndex(unsigned int idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }
    unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems || idx > uCountOfItems-1)
    {
        return;
    }
    
    CCTableViewCell* cell = cellAtIndex(idx);
    if (cell)
    {
        _moveCellOutOfSight(cell);
    }
    cell = m_pDataSource->tableCellAtIndex(this, idx);
    _setIndexForCell(idx, cell);
    _addCellIfNecessary(cell);
}

void CCGridView::insertCellAtIndex(unsigned  int idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }
    
    unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems || idx > uCountOfItems-1)
    {
        return;
    }
    
    CCTableViewCell* cell = NULL;
    int newIdx = 0;
    
    cell = (CCTableViewCell*)m_pCellsUsed->objectWithObjectID(idx);
    if (cell)
    {
        newIdx = m_pCellsUsed->indexOfSortedObject(cell);
        for (unsigned int i=newIdx; i<m_pCellsUsed->count(); i++)
        {
            cell = (CCTableViewCell*)m_pCellsUsed->objectAtIndex(i);
            _setIndexForCell(cell->getIdx()+1, cell);
        }
    }
    
    //insert a new cell
    cell = m_pDataSource->tableCellAtIndex(this, idx);
    _setIndexForCell(idx, cell);
    _addCellIfNecessary(cell);
    _updateCellPositions();
    _updateContentSize();
}

void CCGridView::removeCellAtIndex(unsigned int idx)
{
    if (idx == CC_INVALID_INDEX)
    {
        return;
    }
    
    unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems || idx > uCountOfItems-1)
    {
        return;
    }
    
    unsigned int newIdx = 0;
    
    CCTableViewCell* cell = cellAtIndex(idx);
    if (!cell)
    {
        return;
    }
    
    newIdx = m_pCellsUsed->indexOfSortedObject(cell);
    
    //remove first
    _moveCellOutOfSight(cell);
    
    m_pIndices->erase(idx);

    for (unsigned int i=m_pCellsUsed->count()-1; i > newIdx; i--)
    {
        cell = (CCTableViewCell*)m_pCellsUsed->objectAtIndex(i);
        _setIndexForCell(cell->getIdx()-1, cell);
    }
}

CCTableViewCell *CCGridView::dequeueCell()
{
    CCTableViewCell *cell;
    
    if (m_pCellsFreed->count() == 0) {
        cell = NULL;
    } else {
        cell = (CCTableViewCell*)m_pCellsFreed->objectAtIndex(0);
        cell->retain();
        m_pCellsFreed->removeObjectAtIndex(0);
        cell->autorelease();
    }
    return cell;
}

void CCGridView::_addCellIfNecessary(CCTableViewCell * cell)
{
    if (cell->getParent() != getContainer())
    {
        getContainer()->addChild(cell);
    }
    m_pCellsUsed->insertSortedObject(cell);
    m_pIndices->insert(cell->getIdx());
}

void CCGridView::_updateCellPositions() {
    int cellsCount = m_pDataSource->numberOfCellsInTableView(this);
    m_vCellsPositions.clear();
    m_hCellsPositions.clear();
    
    CCSize cellSize;
    float pos;
    CCScrollViewDirection d = getDirection();
    switch(d) {
        case kCCScrollViewDirectionHorizontal:
            // view rows
            pos = 0;
            for (int i = 0; i < cellsCount; i++) {
                cellSize = m_pDataSource->tableCellSizeForIndex(this, i);
                pos += cellSize.height;
                if(pos >= m_tViewSize.height) {
                    m_viewRows = i + 1;
                    break;
                }
            }
            
            // v pos
            pos = 0;
            for (int i = 0; i < m_viewRows; i++) {
                cellSize = m_pDataSource->tableCellSizeForIndex(this, i);
                m_vCellsPositions.push_back(pos);
                pos += cellSize.height;
            }
            m_vCellsPositions.push_back(pos);
            
            // h pos
            pos = 0;
            for (int i = 0; i < cellsCount; i += m_viewRows) {
                cellSize = m_pDataSource->tableCellSizeForIndex(this, i);
                m_hCellsPositions.push_back(pos);
                pos += cellSize.width;
            }
            m_hCellsPositions.push_back(pos);
            break;
        default:
        {
            // cols and rows
            int cols = m_colCount;
            
            // v pos
            pos = 0;
            for (int i = 0; i < cellsCount; i += cols) {
                cellSize = m_pDataSource->tableCellSizeForIndex(this, i);
                m_vCellsPositions.push_back(pos);
                pos += cellSize.height;
            }
            m_vCellsPositions.push_back(pos);
            
            // h pos
            pos = 0;
            for (int i = 0; i < cols; i++) {
                cellSize = m_pDataSource->tableCellSizeForIndex(this, i);
                m_hCellsPositions.push_back(pos);
                pos += cellSize.width;
            }
            m_hCellsPositions.push_back(pos);
            break;
        }
    }
}

void CCGridView::_updateContentSize() {
    CCSize contentSize;
    contentSize.width = MAX(m_hCellsPositions[m_hCellsPositions.size() - 1], m_tViewSize.width);
    contentSize.height = MAX(m_vCellsPositions[m_vCellsPositions.size() - 1], m_tViewSize.height);
    setContentSize(contentSize);
    
	if (m_eOldDirection != m_eDirection)
	{
		if (m_eDirection == kCCScrollViewDirectionHorizontal)
		{
			setContentOffset(ccp(0,0));
		}
		else
		{
			setContentOffset(ccp(0,minContainerOffset().y));
		}
		m_eOldDirection = m_eDirection;
	}
    
}

CCPoint CCGridView::_offsetFromIndex(unsigned int index) {
    const CCSize& contentSize = getContentSize();
    switch (getDirection()) {
        case kCCScrollViewDirectionHorizontal:
        {
            int row = index % m_viewRows;
            int col = index / m_viewRows;
            return ccp(m_hCellsPositions[col],
                       contentSize.height - m_vCellsPositions[row + 1] + 1);
        }
        default:
        {
            int row = index / m_colCount;
            int col = index % m_colCount;
            return ccp(m_hCellsPositions[col],
                       contentSize.height - m_vCellsPositions[row + 1] + 1);
        }
    }
}

int CCGridView::_indexFromOffset(CCPoint offset) {
    // max index
    const int maxIdx = m_pDataSource->numberOfCellsInTableView(this) - 1;
    
    // locate
    const CCSize& contentSize = getContentSize();
    offset.y = contentSize.height - offset.y;
    int c = (int)m_hCellsPositions.size();
    int row, col = c;
    for(int i = 0; i < c; i++) {
        if(offset.x < m_hCellsPositions[i]) {
            col = MAX(0, i - 1);
            break;
        } else if(offset.x == m_hCellsPositions[i]) {
            col = MAX(0, i);
            break;
        }
    }
    c = (int)m_vCellsPositions.size();
    row = c;
    for(int i = 0; i < c; i++) {
        if(offset.y < m_vCellsPositions[i]) {
            row = MAX(0, i - 1);
            break;
        } else if(offset.y == m_vCellsPositions[i]) {
            row = MAX(0, i);
            break;
        }
    }
    
    // get index, without range limit
    int index = -1;
    switch (getDirection()) {
        case kCCScrollViewDirectionHorizontal:
            index = row + m_viewRows * col;
            break;
        default:
            index = col + row * m_colCount;
            break;
    }
    
    // range checking
    index = MAX(0, index);
    if(index > maxIdx) {
        index = CC_INVALID_INDEX;
    }
    
    return index;
}

void CCGridView::_moveCellOutOfSight(CCTableViewCell *cell)
{
    if(m_pTableViewDelegate != NULL) {
        m_pTableViewDelegate->tableCellWillRecycle(this, cell);
    }
    
    m_pCellsFreed->addObject(cell);
    m_pCellsUsed->removeSortedObject(cell);
    m_pIndices->erase(cell->getIdx());
    // [m_pIndices removeIndex:cell.idx];
    cell->reset();
    if (cell->getParent() == getContainer()) {
        getContainer()->removeChild(cell, true);;
    }
}

void CCGridView::_setIndexForCell(unsigned int index, CCTableViewCell *cell)
{
    cell->setAnchorPoint(ccp(0.0f, 0.0f));
    cell->setPosition(_offsetFromIndex(index));
    cell->setIdx(index);
}

void CCGridView::scrollViewDidScroll(CCScrollView* view)
{
    unsigned int uCountOfItems = m_pDataSource->numberOfCellsInTableView(this);
    if (0 == uCountOfItems)
    {
        return;
    }
    
    if(m_pTableViewDelegate != NULL) {
        m_pTableViewDelegate->scrollViewDidScroll(this);
    }
    
    unsigned int startIdx = 0, endIdx = 0, idx = 0, maxIdx = 0;
    CCPoint offset = ccpMult(getContentOffset(), -1);
    maxIdx = MAX(uCountOfItems-1, 0);
    
    offset.y = offset.y + m_tViewSize.height/getContainer()->getScaleY();
    startIdx = _indexFromOffset(offset);
	if (startIdx == CC_INVALID_INDEX)
	{
		startIdx = uCountOfItems - 1;
	}
    
    offset.y -= m_tViewSize.height/getContainer()->getScaleY();
    offset.x += m_tViewSize.width/getContainer()->getScaleX();
    
    endIdx   = _indexFromOffset(offset);
    if (endIdx == CC_INVALID_INDEX)
	{
		endIdx = uCountOfItems - 1;
	}
    
#if 0 // For Testing.
    CCObject* pObj;
    int i = 0;
    CCARRAY_FOREACH(m_pCellsUsed, pObj)
    {
        CCTableViewCell* pCell = (CCTableViewCell*)pObj;
        CCLog("cells Used index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    CCLog("---------------------------------------");
    i = 0;
    CCARRAY_FOREACH(m_pCellsFreed, pObj)
    {
        CCTableViewCell* pCell = (CCTableViewCell*)pObj;
        CCLog("cells freed index %d, value = %d", i, pCell->getIdx());
        i++;
    }
    CCLog("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
#endif
    
    if (m_pCellsUsed->count() > 0)
    {
        CCTableViewCell* cell = (CCTableViewCell*)m_pCellsUsed->objectAtIndex(0);
        
        idx = cell->getIdx();
        while(idx <startIdx)
        {
            _moveCellOutOfSight(cell);
            if (m_pCellsUsed->count() > 0)
            {
                cell = (CCTableViewCell*)m_pCellsUsed->objectAtIndex(0);
                idx = cell->getIdx();
            }
            else
            {
                break;
            }
        }
    }
    if (m_pCellsUsed->count() > 0)
    {
        CCTableViewCell *cell = (CCTableViewCell*)m_pCellsUsed->lastObject();
        idx = cell->getIdx();
        
        while(idx <= maxIdx && idx > endIdx)
        {
            _moveCellOutOfSight(cell);
            if (m_pCellsUsed->count() > 0)
            {
                cell = (CCTableViewCell*)m_pCellsUsed->lastObject();
                idx = cell->getIdx();
                
            }
            else
            {
                break;
            }
        }
    }
    
    for (unsigned int i=startIdx; i <= endIdx; i++)
    {
        //if ([m_pIndices containsIndex:i])
        if (m_pIndices->find(i) != m_pIndices->end())
        {
            continue;
        }
        updateCellAtIndex(i);
    }
}

void CCGridView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!isVisible()) {
        return;
    }
    
    if (m_pTouchedCell){
		CCRect bb = boundingBox();
		bb.origin = m_pParent->convertToWorldSpace(bb.origin);
        
		if (bb.containsPoint(pTouch->getLocation()) && m_pTableViewDelegate != NULL)
        {
            m_pTableViewDelegate->tableCellUnhighlight(this, m_pTouchedCell);
            m_pTableViewDelegate->tableCellTouched(this, m_pTouchedCell);
        }
        
        m_pTouchedCell = NULL;
    }
    
    CCScrollView::ccTouchEnded(pTouch, pEvent);
}

bool CCGridView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!isVisible()) {
        return false;
    }
    
    bool touchResult = CCScrollView::ccTouchBegan(pTouch, pEvent);
    
    if(m_pTouches->count() == 1) {
        unsigned int        index;
        CCPoint           point;
        
        point = getContainer()->convertTouchToNodeSpace(pTouch);
        
        index = _indexFromOffset(point);
		if (index == CC_INVALID_INDEX)
		{
			m_pTouchedCell = NULL;
		}
        else
		{
			m_pTouchedCell  = cellAtIndex(index);
		}
        
        if (m_pTouchedCell && m_pTableViewDelegate != NULL) {
            m_pTableViewDelegate->tableCellHighlight(this, m_pTouchedCell);
        }
    }
    else if(m_pTouchedCell) {
        if(m_pTableViewDelegate != NULL) {
            m_pTableViewDelegate->tableCellUnhighlight(this, m_pTouchedCell);
        }
        
        m_pTouchedCell = NULL;
    }
    
    return touchResult;
}

void CCGridView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    CCScrollView::ccTouchMoved(pTouch, pEvent);
    
    if (m_pTouchedCell && isTouchMoved()) {
        if(m_pTableViewDelegate != NULL) {
            m_pTableViewDelegate->tableCellUnhighlight(this, m_pTouchedCell);
        }
        
        m_pTouchedCell = NULL;
    }
}

void CCGridView::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    CCScrollView::ccTouchCancelled(pTouch, pEvent);
    
    if (m_pTouchedCell) {
        if(m_pTableViewDelegate != NULL) {
            m_pTableViewDelegate->tableCellUnhighlight(this, m_pTouchedCell);
        }
        
        m_pTouchedCell = NULL;
    }
}

void CCGridView::unregisterAllScriptHandler()
{
    unregisterScriptHandler(kTableViewScroll);
    unregisterScriptHandler(kTableViewZoom);
    unregisterScriptHandler(kTableCellTouched);
    unregisterScriptHandler(kTableCellHighLight);
    unregisterScriptHandler(kTableCellUnhighLight);
    unregisterScriptHandler(kTableCellWillRecycle);
    unregisterScriptHandler(kTableCellSizeForIndex);
    unregisterScriptHandler(kTableCellSizeAtIndex);
    unregisterScriptHandler(kNumberOfCellsInTableView);
}

void CCGridView::setColCount(unsigned int cols) {
    m_colCount = cols;
    _updateCellPositions();
    _updateContentSize();
}

int CCGridView::getRealRows() {
    return (int)m_vCellsPositions.size() - 1;
}

NS_CC_END