/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkTriangleCell.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 2000 National Library of Medicine
  All rights reserved.

  See COPYRIGHT.txt for copyright details.

=========================================================================*/
// #include "itkTriangleCell.h"

/**
 *
 */
template <typename TPixelType, typename TMeshType>
itkTriangleCell< TPixelType , TMeshType >::Pointer
itkTriangleCell< TPixelType , TMeshType >
::New(void)
{
  return new Self;
}


/**
 * Get the topological dimension of this cell.
 */
template <typename TPixelType, typename TMeshType>
int
itkTriangleCell< TPixelType , TMeshType >
::GetCellDimension(void)
{
  return CellDimension;
}


/**
 * Get the number of boundary features of the given dimension.
 */
template <typename TPixelType, typename TMeshType>
itkTriangleCell< TPixelType , TMeshType >::CellFeatureCount
itkTriangleCell< TPixelType , TMeshType >
::GetNumberOfBoundaryFeatures(int dimension)
{
  switch (dimension)
    {
    case 0: return GetNumberOfVertices();
    case 1: return GetNumberOfEdges();
    default: return 0;
    }
}


/**
 * Get the boundary feature of the given dimension specified by the given
 * cell feature Id.
 */
template <typename TPixelType, typename TMeshType>
itkTriangleCell< TPixelType , TMeshType >::Cell::Pointer
itkTriangleCell< TPixelType , TMeshType >
::GetBoundaryFeature(int dimension, CellFeatureIdentifier featureId)
{
  switch (dimension)
    {
    case 0: return Cell::Pointer(GetCellVertex(featureId));
    case 1: return Cell::Pointer(GetCellEdge(featureId));
    default: return Cell::Pointer(NULL);
    }
}


/**
 * Standard itkCell API:
 * Set the cell's internal point list to the list of identifiers provided.
 */
template <typename TPixelType, typename TMeshType>
void
itkTriangleCell< TPixelType , TMeshType >
::SetCellPoints(const PointIdentifier *ptList)
{
  for(int i=0; i < NumberOfPoints ; ++i)
    m_PointIds[i] = ptList[i];
}


/**
 * Standard itkCell API:
 * Use this to set all the points in the cell.  It is assumed that the
 * range [first, last) is exactly the size needed for this cell type.
 * The position *last is NOT referenced, so it can safely be one beyond
 * the end of an array.
 */
template <typename TPixelType, typename TMeshType>
void
itkTriangleCell< TPixelType , TMeshType >
::SetCellPoints(const PointIdentifier* first, const PointIdentifier* last)
{
  int localId=0;
  const PointIdentifier *ii = first;
  
  while(ii != last)
    m_PointIds[localId++] = *ii++;
}


/**
 * Use this to set an individual point identifier in the cell.
 */
template <typename TPixelType, typename TMeshType>
void
itkTriangleCell< TPixelType , TMeshType >
::SetCellPoint(int localId, PointIdentifier ptId)
{
  m_PointIds[localId] = ptId;
}


/**
 * Triangle-specific:
 * Get the number of vertices defining the triangle.
 */
template <typename TPixelType, typename TMeshType>
itkTriangleCell< TPixelType , TMeshType >::CellFeatureCount
itkTriangleCell< TPixelType , TMeshType >
::GetNumberOfVertices(void)
{
  return NumberOfVertices;
}


/**
 * Triangle-specific:
 * Get the number of edges defined for the triangle.
 */
template <typename TPixelType, typename TMeshType>
itkTriangleCell< TPixelType , TMeshType >::CellFeatureCount
itkTriangleCell< TPixelType , TMeshType >
::GetNumberOfEdges(void)
{
  return NumberOfEdges;
}


/**
 * Triangle-specific:
 * Get the vertex specified by the given cell feature Id.
 */
template <typename TPixelType, typename TMeshType>
itkTriangleCell< TPixelType , TMeshType >::Vertex::Pointer
itkTriangleCell< TPixelType , TMeshType >
::GetCellVertex(CellFeatureIdentifier vertexId)
{
  Vertex::Pointer vert(Vertex::New());
  vert->SetCellPoint(0, m_PointIds[vertexId]);
  
  return vert;
}


/**
 * Triangle-specific:
 * Get the edge specified by the given cell feature Id.
 */
template <typename TPixelType, typename TMeshType>
itkTriangleCell< TPixelType , TMeshType >::Edge::Pointer
itkTriangleCell< TPixelType , TMeshType >
::GetCellEdge(CellFeatureIdentifier edgeId)
{
  Edge::Pointer edge(Edge::New());
  
  for(int i=0; i < Edge::NumberOfPoints; ++i)
    edge->SetCellPoint(i, m_PointIds[ m_Edges[edgeId][i] ]);
  
  return edge;
}


/**
 * Define the triangle's topology data.
 */
template <typename TPixelType, typename TMeshType>
const int
itkTriangleCell< TPixelType , TMeshType >
::m_Edges[3][2] = { {0,1}, {1,2}, {2,0} };

