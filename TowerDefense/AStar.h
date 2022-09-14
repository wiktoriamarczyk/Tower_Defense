#pragma once
#include "Common.h"

struct AStarMapCell
{
    vec2i               CellPosition;
    const AStarMapCell* pPrevCell = nullptr;
    bool                IsVisited = false;
    bool                IsBlocked = false;
};

class AStar
{
public:
    // inicjalizacja klasy - skopiowanie danych mapy do wewnętrznych struktur
    bool InitFinder(const eGridValue (&MapData)[GRID_ROWS][GRID_COLS]);
    bool FindPath(vec2i StartCell, vec2i EndCell, vector<vec2>& FoundPath);

private:
    // reset danych
    void ResetSearchData();
    // pobranie komórki
    AStarMapCell* GetCell (vec2i CellPoS);
    // sprawdzenie poprawności danej komórki
    bool IsValidCell(vec2i CellPos);
    // dodanie komórki do listy sprawdzanych komórek
    void AddToSearchList(const AStarMapCell* pPrevCell, vec2i CellPos);

    AStarMapCell         m_MapData[GRID_ROWS][GRID_COLS];  //< dwuwymiarowa mapa komórek
    deque<AStarMapCell*> m_WorkList;                       //< lista wskaźników na komórki do sprawdzenia

    bool m_Initialize = false;

};