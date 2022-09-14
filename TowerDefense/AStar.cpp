#include "AStar.h"

bool AStar::InitFinder(const eGridValue (&MapData)[GRID_ROWS][GRID_COLS])
{
    for (int y = 0;  y < GRID_ROWS; ++y)
    {
        for (int x = 0; x < GRID_COLS; ++x)
        {
            m_MapData[y][x].CellPosition = vec2i(x, y);
            m_MapData[y][x].IsBlocked = (MapData[y][x] != eGridValue::PATH) ? true : false;
        }
    }
    m_Initialize = true;
    return true;
}

bool AStar::FindPath( vec2i MapCellStart , vec2i MapCellEnd , vector<vec2>& FoundPath )
{
    // czyszczenie stanu komorek
    ResetSearchData();

    if (!m_Initialize)
        return false;

    MapCellStart = MapCellStart / CELL_SIZE;
    MapCellEnd = MapCellEnd / CELL_SIZE;

    if (MapCellStart == MapCellEnd)
        return true;

    // dodanie komorki startowej do listy sprawdzanych komorek
    AddToSearchList(nullptr, MapCellStart);

    while (m_WorkList.size())
    {
        m_WorkList[0]->IsVisited = true;

        if (m_WorkList[0]->CellPosition == MapCellEnd)
        {
            // stworzenie sciezki (listy komorek), ktora doprowadzila nas do komorki docelowej, wykorzystujac wskaznik na poprzednika kazdej komorki
            for (const AStarMapCell* Current = m_WorkList[0]; Current != nullptr; Current = Current->pPrevCell)
            {
                FoundPath.insert(FoundPath.begin(), Current->CellPosition * CELL_SIZE);
            }
            // odnaleziono sciezke do komorki docelowej
            return true;
        }

        // dodanie do listy sprawdzanych komorek wszystkich sasiadow aktualnie sprawdzanej komorki
        AddToSearchList(m_WorkList[0], vec2i(m_WorkList[0]->CellPosition.x - 1, m_WorkList[0]->CellPosition.y));
        AddToSearchList(m_WorkList[0], vec2i(m_WorkList[0]->CellPosition.x + 1, m_WorkList[0]->CellPosition.y));
        AddToSearchList(m_WorkList[0], vec2i(m_WorkList[0]->CellPosition.x, m_WorkList[0]->CellPosition.y - 1));
        AddToSearchList(m_WorkList[0], vec2i(m_WorkList[0]->CellPosition.x, m_WorkList[0]->CellPosition.y + 1));

        // usuniecie z listy sprawdzanych komorek aktualnie sprawdzanej komorki
        m_WorkList.pop_front();
        //m_WorkList.erase(m_WorkList.begin());
    }

    return false;
}

void AStar::ResetSearchData()
{
    for (int y = 0;  y < GRID_ROWS; ++y)
    {
        for (int x = 0; x < GRID_COLS; ++x)
        {
            m_MapData[y][x].pPrevCell = nullptr;
            m_MapData[y][x].IsVisited = false;
        }
    }

    m_WorkList.clear();
}

AStarMapCell* AStar::GetCell( vec2i CellPos )
{
    // ta funkcja powinna zwrócić wskaźnik na komórkę: &( m_MapData[ Y ][ X ] ), JEŚLI pozycja jest poprawna,
    // jeśli pozycja jest poza mapą, to powinnismy zwrócić nullptr

    if (CellPos.x < 0 || CellPos.x >= GRID_COLS || CellPos.y < 0 || CellPos.y >= GRID_ROWS)
        return nullptr;

    return &(m_MapData[CellPos.y][CellPos.x]);
}

bool AStar::IsValidCell( vec2i CellPos )
{
    // Ta funkcja powinna sprawdzić czy na podanej pozycji jest poprawna komórka, czyli:
    // - czy nie wychodzi poza mapę
    // - czy nie była już odwiedzona
    // - czy nie jest zablokowana

    if (!GetCell(CellPos))
        return false;

    if (m_MapData[CellPos.y][CellPos.x].IsVisited)
        return false;

    if (m_MapData[CellPos.y][CellPos.x].IsBlocked)
        return false;

    return true;
}

void AStar::AddToSearchList( const AStarMapCell* pPrevCell , vec2i CellPos )
{
    if (!IsValidCell(CellPos))
        return;

    AStarMapCell* tmp = GetCell(CellPos);

    tmp->pPrevCell = pPrevCell;

    m_WorkList.push_back(tmp);
}