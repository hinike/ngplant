/***************************************************************************

 Copyright (c) 2007 Sergey Prokhorchuk.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
 3. Neither the name of the author nor the names of contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.

***************************************************************************/

#ifndef __P3DHLI_H__
#define __P3DHLI_H__

#include <ngpcore/p3dmathrng.h>
#include <ngpcore/p3dmodel.h>

#define P3DHLI_VER_MAJOR    (0)
#define P3DHLI_VER_MINOR    (9)
#define P3DHLI_VER_RELEASE  (11)

typedef float *(P3DHLIVAttrBufferSet[P3D_MAX_ATTRS]);

class P3D_DLL_ENTRY P3DHLIVAttrBuffers
 {
  public           :

                   P3DHLIVAttrBuffers ();

  void             AddAttr            (unsigned int        Attr,
                                       void               *Data,
                                       unsigned int        Offset,
                                       unsigned int        Stride);

  bool             HasAttr            (unsigned int        Attr) const;
  void            *GetAttrBuffer      (unsigned int        Attr) const;
  unsigned int     GetAttrOffset      (unsigned int        Attr) const;
  unsigned int     GetAttrStride      (unsigned int        Attr) const;

  private          :

  void            *Buffers[P3D_MAX_ATTRS];
  unsigned int     Offsets[P3D_MAX_ATTRS];
  unsigned int     Strides[P3D_MAX_ATTRS];
 };

/******************************************************************************/
/* NOTE: Class P3DHLIVAttrFormat and P3DHLIPlantInstance::FillVAttrBufferI    */
/* are obsolete. Both of them will be removed in one of the next version.     */
/* Please, use P3DHLIVAttrBuffers and  P3DHLIPlantInstance::FillVAttrBuffersI */
/* instead.                                                                   */
/******************************************************************************/

class P3D_DLL_ENTRY P3DHLIVAttrFormat
 {
  public           :

                   P3DHLIVAttrFormat  (unsigned int        Stride);

  bool             HasAttr            (unsigned int        Attr) const;
  unsigned int     GetAttrOffset      (unsigned int        Attr) const;
  unsigned int     GetStride          () const;
  void             AddAttr            (unsigned int        Attr,
                                       unsigned int        Offset);

  private          :

  bool             Enabled[P3D_MAX_ATTRS];
  unsigned int     Offsets[P3D_MAX_ATTRS];
  unsigned int     Stride;
 };

class P3DHLIPlantInstance;

class P3D_DLL_ENTRY P3DHLIPlantTemplate
 {
  public           :

                   P3DHLIPlantTemplate(P3DInputStringStream
                                                          *SourceStream);
                   P3DHLIPlantTemplate(const P3DPlantModel*SourceModel);

  const
  P3DModelMetaInfo*GetMetaInfo        () const;

  unsigned int     GetGroupCount      () const;

  const char      *GetGroupName       (unsigned int        GroupIndex) const;

  bool             IsDummy            (unsigned int        GroupIndex) const;

  const
  P3DMaterialDef  *GetMaterial        (unsigned int        GroupIndex) const;

  void             GetBillboardSize   (float              *Width,
                                       float              *Height,
                                       unsigned int        GroupIndex) const;

  bool             IsCloneable        (unsigned int        GroupIndex,
                                       bool                AllowScaling) const;

  /* LOD-related */

  bool             IsLODVisRangeEnabled
                                      (unsigned int        GroupIndex) const;

  void             GetLODVisRange     (float              *MinLOD,
                                       float              *MaxLOD,
                                       unsigned int        GroupIndex) const;

  /* Per-attribute mode */

  unsigned int     GetVAttrCount      (unsigned int        GroupIndex,
                                       unsigned int        Attr) const;

  void             FillCloneVAttrBuffer
                                      (void               *VAttrBuffer,
                                       unsigned int        GroupIndex,
                                       unsigned int        Attr) const;

  unsigned int     GetPrimitiveCount  (unsigned int        GroupIndex) const;
  unsigned int     GetPrimitiveType   (unsigned int        GroupIndex,
                                       unsigned int        PrimitiveIndex) const;

  void             FillVAttrIndexBuffer
                                      (void               *IndexBuffer,
                                       unsigned int        GroupIndex,
                                       unsigned int        Attr,
                                       unsigned int        ElementType,
                                       unsigned int        IndexBase = 0) const;

  /* Indexed mode */

  unsigned int     GetVAttrCountI     (unsigned int        GroupIndex) const;

  void             FillCloneVAttrBuffersI
                                      (const P3DHLIVAttrBuffers
                                                          *VAttrBuffers,
                                       unsigned int        GroupIndex) const;

  unsigned int     GetIndexCount      (unsigned int        GroupIndex,
                                       unsigned int        PrimitiveType) const;

  void             FillIndexBuffer    (void               *IndexBuffer,
                                       unsigned int        GroupIndex,
                                       unsigned int        PrimitiveType,
                                       unsigned int        ElementType,
                                       unsigned int        IndexBase = 0) const;

  void             SetDummiesEnabled  (bool                Enabled);
  bool             IsDummiesEnabled   () const;

  P3DHLIPlantInstance
                  *CreateInstance     (unsigned int        BaseSeed = 0) const;

  private          :

  const P3DPlantModel                 *Model;
  P3DPlantModel                        OwnedModel;
  bool                                 DummiesEnabled;
 };

class P3D_DLL_ENTRY P3DHLIPlantInstance
 {
  public           :

                   P3DHLIPlantInstance(const P3DPlantModel*Model,
                                       unsigned int        BaseSeed,
                                       bool                DummiesEnabled);

  unsigned int     GetBranchCount     (unsigned int        GroupIndex) const;
  void             GetBranchCountMulti(unsigned int       *BranchCounts) const;
  void             GetBoundingBox     (float              *Min,
                                       float              *Max) const;

  /* Per-clone mode (use only for "cloneable" groups) */

  /* size of OffsetBuffer      must be sizeof(float) * GetBranchCount() * 3 */
  /* size of OrientationBuffer must be sizeof(float) * GetBranchCount() * 4 */
  /* size of ScaleBuffer       must be sizeof(float) * GetBranchCount()     */
  void             FillCloneTransformBuffer
                                      (float              *OffsetBuffer,
                                       float              *OrientationBuffer,
                                       float              *ScaleBuffer,
                                       unsigned int        GroupIndex) const;

  /* Per-attribute mode */

  unsigned int     GetVAttrCount      (unsigned int        GroupIndex,
                                       unsigned int        Attr) const;

  void             FillVAttrBuffer    (void               *VAttrBuffer,
                                       unsigned int        GroupIndex,
                                       unsigned int        Attr) const;

  /* Indexed mode */

  unsigned int     GetVAttrCountI     (unsigned int        GroupIndex) const;

  void             FillVAttrBufferI   (void               *VAttrBuffer,
                                       unsigned int        GroupIndex,
                                       const P3DHLIVAttrFormat
                                                          *VAttrFormat) const;

  void             FillVAttrBuffersI  (const P3DHLIVAttrBuffers
                                                          *VAttrBuffers,
                                       unsigned int        GroupIndex) const;

  void             FillVAttrBuffersIMulti
                                      (P3DHLIVAttrBufferSet
                                                          *VAttrBufferSet) const;

  private          :

  bool             IsRandomnessEnabled() const;

  const P3DPlantModel                 *Model;
  unsigned int                         BaseSeed;
  bool                                 DummiesEnabled;
 };

#endif

