/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkWeightedMeanSampleFilter_txx
#define __itkWeightedMeanSampleFilter_txx

#include "itkMeasurementVectorTraits.h"

namespace itk
{
namespace Statistics
{
template< class TSample >
WeightedMeanSampleFilter< TSample >
::WeightedMeanSampleFilter()
{
  this->ProcessObject::SetNthInput(1, NULL);
}

template< class TSample >
WeightedMeanSampleFilter< TSample >
::~WeightedMeanSampleFilter()
{}

template< class TSample >
void
WeightedMeanSampleFilter< TSample >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  // m_Weights
  os << indent << "Weights: " << this->GetWeightsInput() << std::endl;
  // m_WeightingFunction
  os << indent << "Weighting Function: " << this->GetWeightingFunctionInput() << std::endl;
}

template< class TSample >
void
WeightedMeanSampleFilter< TSample >
::GenerateData()
{
  // if weighting function is specifed, use it to compute the mean
  const InputWeightingFunctionObjectType *functionObject =
    this->GetWeightingFunctionInput();

  if ( functionObject != NULL )
    {
    this->ComputeMeanWithWeightingFunction();
    return;
    }

  // if weight array is specified use it to compute the mean
  const InputWeightArrayObjectType *weightArrayObject =
    this->GetWeightsInput();

  if ( weightArrayObject != NULL )
    {
    this->ComputeMeanWithWeights();
    return;
    }

  // Otherwise compute the regular mean ( without weight coefficients)
  Superclass::GenerateData();
  return;
}

template< class TSample >
void
WeightedMeanSampleFilter< TSample >
::ComputeMeanWithWeights()
{
  const SampleType *input = this->GetInput();

  MeasurementVectorSizeType measurementVectorSize =
    input->GetMeasurementVectorSize();

  MeasurementVectorDecoratedType *decoratedOutput =
    static_cast< MeasurementVectorDecoratedType * >(
      this->ProcessObject::GetOutput(0) );

  MeasurementVectorType output = decoratedOutput->Get();

  //reset the output
  for ( unsigned int dim = 0; dim < measurementVectorSize; dim++ )
    {
    output[dim] = NumericTraits< MeasurementType >::Zero;
    }

  typename TSample::ConstIterator iter = input->Begin();
  typename TSample::ConstIterator end =  input->End();
  double totalWeight = 0.0;
  double weight;

  typename TSample::MeasurementVectorType measurements;

  const InputWeightArrayObjectType *weightArrayObject = this->GetWeightsInput();
  const WeightArrayType             weightArray = weightArrayObject->Get();

  int measurementVectorIndex = 0;

  while ( iter != end )
    {
    measurements = iter.GetMeasurementVector();
    weight = iter.GetFrequency() * ( weightArray )[measurementVectorIndex];
    totalWeight += weight;

    for ( unsigned int dim = 0; dim < measurementVectorSize; dim++ )
      {
      output[dim] += measurements[dim] * weight;
      }
    ++measurementVectorIndex;
    ++iter;
    }

  if ( totalWeight != 0.0 )
    {
    for ( unsigned int dim = 0; dim < measurementVectorSize; dim++ )
      {
      output[dim] /= totalWeight;
      }
    }

  decoratedOutput->Set(output);
}

template< class TSample >
void
WeightedMeanSampleFilter< TSample >
::ComputeMeanWithWeightingFunction()
{
  const SampleType *input = this->GetInput();

  MeasurementVectorSizeType measurementVectorSize =
    input->GetMeasurementVectorSize();

  MeasurementVectorDecoratedType *decoratedOutput =
    static_cast< MeasurementVectorDecoratedType * >(
      this->ProcessObject::GetOutput(0) );

  MeasurementVectorType output = decoratedOutput->Get();

  //reset the output
  for ( unsigned int dim = 0; dim < measurementVectorSize; dim++ )
    {
    output[dim] = NumericTraits< MeasurementType >::Zero;
    }

  typename TSample::ConstIterator iter = input->Begin();
  typename TSample::ConstIterator end =  input->End();
  double totalWeight = 0.0;
  double weight;

  typename TSample::MeasurementVectorType measurements;

  // if weighting function is specifed, use it to compute the mean
  const InputWeightingFunctionObjectType *functionObject =
    this->GetWeightingFunctionInput();

  const WeightingFunctionType *weightFunction = functionObject->Get();

  while ( iter != end )
    {
    measurements = iter.GetMeasurementVector();
    weight =
      iter.GetFrequency() * weightFunction->Evaluate(measurements);
    totalWeight += weight;
    for ( unsigned int dim = 0; dim < measurementVectorSize; dim++ )
      {
      output[dim] += measurements[dim] * weight;
      }
    ++iter;
    }

  if ( totalWeight != 0.0 )
    {
    for ( unsigned int dim = 0; dim < measurementVectorSize; dim++ )
      {
      output[dim] /= totalWeight;
      }
    }

  decoratedOutput->Set(output);
}
} // end of namespace Statistics
} // end of namespace itk

#endif
