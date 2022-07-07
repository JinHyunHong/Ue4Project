#pragma once

#define CheckNull(p) {if(p == NULL) return;}
#define CheckNullResult(p, result) {if(p == NULL) return result;}
#define CheckNullBreak(p) {if(p == NULL) break;}
#define CheckNullContinue(p) {if(p == NULL) continue;}

#define CheckTrue(p) {if(p == true) return;}
#define CheckTrueResult(p, result) {if(p == true) return result;}
#define CheckTrueBreak(p) {if(p == true) break;}
#define CheckTrueContinue(p) {if(p == true) continue;}

#define CheckFalse(p) {if(p == false) return;}
#define CheckFalseResult(p, result) {if(p == false) return result;}
#define CheckFalseBreak(p) {if(p == false) break;}
#define CheckFalseContinue(p) {if(p == false) continue;}

#define CheckEmpty(p) {if(p == "") return;}
#define CheckEmptyResult(p, result) {if(p == "") return result;}
#define CheckEmptyBreak(p) {if(p == "") break;}
#define CheckEmptyContinue(p) {if(p == "") continue;}