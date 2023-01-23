/*******************************************************************************
    Copyright (C) 2021 Kevin Sahr

    This file is part of DGGRID.

    DGGRID is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DGGRID is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
//
// appex.cpp: simple application that demonstrates using the dglib library to
//            manipulate DGG cells.
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

using namespace std;

#include <dglib/DgIDGGS4H.h>
//#include <dglib/DgIDGGS.h>
#include <dglib/DgBoundedIDGGS.h>


////////////////////////////////////////////////////////////////////////////////
int main (int, char**)
{
   ///// create the DGG /////

   // create the reference frame (RF) conversion network
   DgRFNetwork net0;

   // create the geodetic reference frame
   // reference frames must be created dynamically using makeRF
   // they will be deleted by the Network
   const DgGeoSphRF& geoRF = *(DgGeoSphRF::makeRF(net0, "GS0"));

   // create the ISEA4H grid system with resolutions 0-19; requires a
   // fixed icosahedron vertex and edge azimuth
   DgGeoCoord vert0(11.25L, 58.28252559L, false); // args: lon, lat, isRadians
   long double azimuth = 0.0L;

   // all DGGS's must be created using a factory makeRF method
   // the DGGS is memory managed by the DgRFNetwork
   const DgIDGGS4H* idggsPtr = DgIDGGS4H::makeRF(net0, geoRF, vert0, azimuth, 20);
   const DgIDGGS4H& idggs = *idggsPtr;

   // get the resolution 18 dgg from the dggs
   const DgIDGG& dgg = idggs.idgg(18);
   cout << dgg.gridStats() << endl;
   //////// now use the DGG /////////

   ///// given a point in lon/lat, get the cell it lies within /////

   // first create a DgLocation in geoRF coordinates
   // DgGeoCoord geoAddress(-122.7083, 42.1947, false);
   DgGeoCoord geoAddress(27.2460, 63.13800, false);


     //DgGeoCoord geoAddress(27.2460 + i/1e3 , 63.13800+i/1e3, false);
     DgLocation* thePt = geoRF.makeLocation(geoAddress);
     cout << "the point " << *thePt << endl;
     // converting the point location to the dgg RF determines which cell it's in
     dgg.convert(thePt);
     cout << "* lies in cell " << *thePt << endl;

     //seqNum address
     DgLocation* thePt2;
     long int sNum = dgg.bndRF().seqNum(*thePt);
     for (long int i = 0; i < 10; i++){
          long int cellnum = sNum + i;
          DgQ2DICoord q2di = dgg.bndRF().addFromSeqNum(cellnum);
          cout << "SeqNum: " << sNum << endl;
          cout << "q2di: " << q2di << endl;
          thePt2 = dgg.makeLocation(q2di);
          cout << "the point lies in cell " << *thePt2 << endl;
     }
     delete thePt;
     delete thePt2;

   // we can get the cell's vertices, which are defined in geoRF
   /*
   DgPolygon verts;
   int ptsPerEdgeDensify = 3;
   dgg.setVertices(*thePt, verts, ptsPerEdgeDensify);
   cout << "* with densified cell boundary:\n" << verts << endl;

   // we can get the cell's center point by converting the cell back to geoRF
   geoRF.convert(thePt);
   cout << "* and cell center point:" << *thePt << endl;

   // we can extract the coordinates into primitive data types
   const DgGeoCoord& centCoord = *geoRF.getAddress(*thePt);
   double latRads = centCoord.lat();
   double lonRads = centCoord.lon();
   cout << "* center point lon,lat in radians: "
        << lonRads << ", " << latRads << endl;

   const DgGeoCoord& firstVert = *geoRF.getAddress(verts[0]);
   double latDegs = firstVert.latDegs();
   double lonDegs = firstVert.lonDegs();
   cout << "* first boundary vertex lon,lat in degrees: "
        << lonDegs << ", " << latDegs << endl;
   */


   return 0;

} // main

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
