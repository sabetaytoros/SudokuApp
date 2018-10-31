/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Ribbon.h
 * Author: Sabetay Tporos
 *
 * Created on 31 October 2018, 12:11
 */

#ifndef RIBBON_H
#define RIBBON_H

/******************************************************************************/
struct TRibbon {
                TRibbon(TSudoku *O, Uint n,  TLineType t) noexcept ;
                ~TRibbon() {  } 
 friend ostream& operator<<(ostream& os, const TRibbon& v);    
            void AddCount(TCell& Cell);
            void CheckMissingOtherCell(TCell& Cell); 
            bool CheckMissingCellinOtherLines(TCell& Cell);
            void CheckMissingCellinOtherBoxes(TCell& Cell);
            void CheckMissingThirdDigit(const Uint& d);

         TSudoku &Osdk;             // Owner
           Uint  No;                // Ribbon No
 const TLineType lt;                // Horizontal, Vertical
         TvpLine RvLines;           // Lines
          TvpBox vpBoxes;            // Triples
          TCount Counts { 0 };      // Cell Counts in Ribbon Max 3
            Uint ctn;               // current triple no when Checked

};

#endif /* RIBBON_H */

