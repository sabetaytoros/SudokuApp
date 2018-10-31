/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Line.h
 * Author: Sabetay Toros *
 * Created on 31 October 2018, 13:53
 */

#ifndef LINE_H
#define LINE_H
/******************************************************************************/
struct TLine {   
                 ~TLine() { }                 
                 TLine(TSudoku *O, TRw *l, Uint  n_, TLineType t) noexcept; 
 friend ostream& operator<<(ostream& os, const TLine& v);
            void AddCell(TCell *Item);
            void Solution();
          TvUint GenerateMisCellVect();
          TvUint GenerateMisLocVect();
            bool InsertMissingCell(TDigit& MisDigit);
            void InsertFinalCell();
            void CheckMissingCell(TvUint& vMsCelNo, TvUint& vMsCelLc);
        PNewItem MissingOtherCellsContains(const Uint e, const TvUint& vMisCellNo);
        PNewItem FillTstVectorLineTripleshasMissingCell(TRw& tstV,TDigit& MisCell);
            bool TrytoInsertMisingCell(const Uint e, const TvUint& vMisCellNo, const bool v, const TRw& mv);
            bool RowDatahasaDigit(const Uint Entry);   
            void UpdateRowDataDigit(const Uint i, const Uint No);     // index, Digit No 
            
         TSudoku &Osdk;          // Owner
             TRw &RwData;        // Column or Row Data;                            
      const Uint No;             // Column Row No
 const TLineType lt;             // r Horizontal c Vertical
          TvpBox LnBoxes;        // Boxes 3 pcs
          TvCell Cells;          // Line Cells
};

#endif /* LINE_H */

