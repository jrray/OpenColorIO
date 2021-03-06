/*
Copyright (c) 2003-2010 Sony Pictures Imageworks Inc., et al.
All Rights Reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
* Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
* Neither the name of Sony Pictures Imageworks nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef INCLUDED_OCIO_OPENCOLORTRANSFORMS_H
#define INCLUDED_OCIO_OPENCOLORTRANSFORMS_H

#include "OpenColorTypes.h"

#ifndef OCIO_NAMESPACE_ENTER
#error This header cannot be used directly. Use <OpenColorIO/OpenColorIO.h> instead.
#endif

/*!rst::
C++ Transforms
==============

Typically only needed when creating and/or manipulating configurations
*/

OCIO_NAMESPACE_ENTER
{

    //!rst:: //////////////////////////////////////////////////////////////////
    
    //!cpp:class:: 
    class OCIOEXPORT Transform
    {
    public:
        virtual ~Transform();
        virtual TransformRcPtr createEditableCopy() const = 0;
        
        virtual TransformDirection getDirection() const = 0;
        virtual void setDirection(TransformDirection dir) = 0;
        
    
    private:
        Transform& operator= (const Transform &);
    };
    
    //!cpp:function::
    extern OCIOEXPORT std::ostream& operator<< (std::ostream&, const Transform&);
    
    
    //!rst:: //////////////////////////////////////////////////////////////////
    
    //!cpp:class:: Forward direction wraps the 'expanded' range into the
    // specified, often compressed, range.
    class OCIOEXPORT AllocationTransform : public Transform
    {
    public:
        //!cpp:function::
        static AllocationTransformRcPtr Create();
        
        //!cpp:function::
        virtual TransformRcPtr createEditableCopy() const;
        
        //!cpp:function::
        virtual TransformDirection getDirection() const;
        //!cpp:function::
        virtual void setDirection(TransformDirection dir);
        
        //!cpp:function::
        Allocation getAllocation() const;
        //!cpp:function::
        void setAllocation(Allocation allocation);
        
        //!cpp:function::
        int getNumVars() const;
        //!cpp:function::
        void getVars(float * vars) const;
        //!cpp:function::
        void setVars(int numvars, const float * vars);
    
    private:
        AllocationTransform();
        AllocationTransform(const AllocationTransform &);
        virtual ~AllocationTransform();
        
        AllocationTransform& operator= (const AllocationTransform &);
        
        static void deleter(AllocationTransform* t);
        
        class Impl;
        friend class Impl;
        Impl * m_impl;
        Impl * getImpl() { return m_impl; }
        const Impl * getImpl() const { return m_impl; }
    };
    
    //!cpp:function::
    extern OCIOEXPORT std::ostream& operator<< (std::ostream&, const AllocationTransform&);
    
    
    //!rst:: //////////////////////////////////////////////////////////////////
    
    //!cpp:class:: An implementation of the ASC CDL Transfer Functions and
    // Interchange - Syntax (Based on the version 1.2 document)
    // 
    // .. note::
    //    the clamping portion of the CDL is only applied if a non-identity
    //    power is specified.
    class OCIOEXPORT CDLTransform : public Transform
    {
    public:
        //!cpp:function::
        static CDLTransformRcPtr Create();
        
        //!cpp:function::
        virtual TransformRcPtr createEditableCopy() const;
        
        //!cpp:function::
        virtual TransformDirection getDirection() const;
        //!cpp:function::
        virtual void setDirection(TransformDirection dir);
        
        //!cpp:function::
        bool equals(const ConstCDLTransformRcPtr & other) const;
        
        //!cpp:function::
        const char * getXML() const;
        //!cpp:function::
        void setXML(const char * xml);
        
        //!rst:: **ASC_SOP**
        // 
        // Slope, offset, power::
        //    
        //    out = clamp( (in * slope) + offset ) ^ power
        
        //!cpp:function::
        void setSlope(const float * rgb);
        //!cpp:function::
        void getSlope(float * rgb) const;
        
        //!cpp:function::
        void setOffset(const float * rgb);
        //!cpp:function::
        void getOffset(float * rgb) const;
        
        //!cpp:function::
        void setPower(const float * rgb);
        //!cpp:function::
        void getPower(float * rgb) const;
        
        //!cpp:function::
        void setSOP(const float * vec9);
        //!cpp:function::
        void getSOP(float * vec9) const;
        
        //!rst:: **ASC_SAT**
        
        //!cpp:function::
        void setSat(float sat);
        //!cpp:function::
        float getSat() const;
        
        //!cpp:function:: These are hard-coded, by spec, to r709
        void getSatLumaCoefs(float * rgb) const;
        
        //!rst:: **Metadata**
        // 
        // These do not affect the image processing, but
        // are often useful for pipeline purposes and are
        // included in the serialization.
        
        //!cpp:function:: Unique Identifier for this correction
        void setID(const char * id);
        //!cpp:function::
        const char * getID() const;
        
        //!cpp:function:: Textual description of color correction
        // (stored on the SOP)
        void setDescription(const char * desc);
        //!cpp:function::
        const char * getDescription() const;
    
    private:
        CDLTransform();
        CDLTransform(const CDLTransform &);
        virtual ~CDLTransform();
        
        CDLTransform& operator= (const CDLTransform &);
        
        static void deleter(CDLTransform* t);
        
        class Impl;
        friend class Impl;
        Impl * m_impl;
        Impl * getImpl() { return m_impl; }
        const Impl * getImpl() const { return m_impl; }
    };
    
    //!cpp:function::
    extern OCIOEXPORT std::ostream& operator<< (std::ostream&, const CDLTransform&);
    
    
    //!rst:: //////////////////////////////////////////////////////////////////
    
    //!cpp:class::
    class OCIOEXPORT ColorSpaceTransform : public Transform
    {
    public:
        //!cpp:function::
        static ColorSpaceTransformRcPtr Create();
        
        //!cpp:function::
        virtual TransformRcPtr createEditableCopy() const;
        
        //!cpp:function::
        virtual TransformDirection getDirection() const;
        //!cpp:function::
        virtual void setDirection(TransformDirection dir);
        
        //!cpp:function::
        const char * getSrc() const;
        //!cpp:function::
        void setSrc(const char * src);
        
        //!cpp:function::
        const char * getDst() const;
        //!cpp:function::
        void setDst(const char * dst);
    
    private:
        ColorSpaceTransform();
        ColorSpaceTransform(const ColorSpaceTransform &);
        virtual ~ColorSpaceTransform();
        
        ColorSpaceTransform& operator= (const ColorSpaceTransform &);
        
        static void deleter(ColorSpaceTransform* t);
        
        class Impl;
        friend class Impl;
        Impl * m_impl;
        Impl * getImpl() { return m_impl; }
        const Impl * getImpl() const { return m_impl; }
    };
    
    //!cpp:function::
    extern OCIOEXPORT std::ostream& operator<< (std::ostream&, const ColorSpaceTransform&);
    
    
    //!rst:: //////////////////////////////////////////////////////////////////
    
    //!cpp:class::
    class OCIOEXPORT DisplayTransform : public Transform
    {
    public:
        //!cpp:function::
        static DisplayTransformRcPtr Create();
        
        //!cpp:function::
        virtual TransformRcPtr createEditableCopy() const;
        
        //!cpp:function::
        virtual TransformDirection getDirection() const;
        //!cpp:function::
        virtual void setDirection(TransformDirection dir);
        
        //!cpp:function:: Step 0. Specify the incoming color space
        void setInputColorSpaceName(const char * name);
        //!cpp:function::
        const char * getInputColorSpaceName() const;
        
        //!cpp:function:: Step 1: Apply a Color Correction, in ROLE_SCENE_LINEAR
        void setLinearCC(const ConstTransformRcPtr & cc);
        //!cpp:function::
        ConstTransformRcPtr getLinearCC() const;
        
        //!cpp:function:: Step 2: Apply a color correction, in ROLE_COLOR_TIMING
        void setColorTimingCC(const ConstTransformRcPtr & cc);
        //!cpp:function::
        ConstTransformRcPtr getColorTimingCC() const;
        
        //!cpp:function:: Step 3: Apply the Channel Viewing Swizzle (mtx)
        void setChannelView(const ConstTransformRcPtr & transform);
        //!cpp:function::
        ConstTransformRcPtr getChannelView() const;
        
        //!cpp:function:: Step 4: Apply the output display transform
        void setDisplayColorSpaceName(const char * name);
        //!cpp:function::
        const char * getDisplayColorSpaceName() const;
        
        //!cpp:function:: Step 5: Apply a post display transform color correction
        void setDisplayCC(const ConstTransformRcPtr & cc);
        //!cpp:function::
        ConstTransformRcPtr getDisplayCC() const;
        
    private:
        DisplayTransform();
        DisplayTransform(const DisplayTransform &);
        virtual ~DisplayTransform();
        
        DisplayTransform& operator= (const DisplayTransform &);
        
        static void deleter(DisplayTransform* t);
        
        class Impl;
        friend class Impl;
        Impl * m_impl;
        Impl * getImpl() { return m_impl; }
        const Impl * getImpl() const { return m_impl; }
    };
    
    //!cpp:function::
    extern OCIOEXPORT std::ostream& operator<< (std::ostream&, const DisplayTransform&);
    
    
    //!rst:: //////////////////////////////////////////////////////////////////
    
    //!cpp:class:: Represents exponent transform: pow( clamp(color), value)
    // 
    // If the exponent is 1.0, this will not clamp. Otherwise, the input color
    // will be clamped between [0.0, inf]
    class OCIOEXPORT ExponentTransform : public Transform
    {
    public:
        //!cpp:function::
        static ExponentTransformRcPtr Create();
        
        //!cpp:function::
        virtual TransformRcPtr createEditableCopy() const;
        
        //!cpp:function::
        virtual TransformDirection getDirection() const;
        //!cpp:function::
        virtual void setDirection(TransformDirection dir);
        
        //!cpp:function::
        void setValue(const float * vec4);
        //!cpp:function::
        void getValue(float * vec4) const;
    
    private:
        ExponentTransform();
        ExponentTransform(const ExponentTransform &);
        virtual ~ExponentTransform();
        
        ExponentTransform& operator= (const ExponentTransform &);
        
        static void deleter(ExponentTransform* t);
        
        class Impl;
        friend class Impl;
        Impl * m_impl;
        Impl * getImpl() { return m_impl; }
        const Impl * getImpl() const { return m_impl; }
    };
    
    //!cpp:function::
    extern OCIOEXPORT std::ostream& operator<< (std::ostream&, const ExponentTransform&);
    
    
    //!rst:: //////////////////////////////////////////////////////////////////
    
    //!cpp:class::
    class OCIOEXPORT FileTransform : public Transform
    {
    public:
        //!cpp:function::
        static FileTransformRcPtr Create();
        
        //!cpp:function::
        virtual TransformRcPtr createEditableCopy() const;
        
        //!cpp:function::
        virtual TransformDirection getDirection() const;
        //!cpp:function::
        virtual void setDirection(TransformDirection dir);
        
        //!cpp:function::
        const char * getSrc() const;
        //!cpp:function::
        void setSrc(const char * src);
        
        //!cpp:function::
        const char * getCCCId() const;
        //!cpp:function::
        void setCCCId(const char * id);
        
        //!cpp:function::
        Interpolation getInterpolation() const;
        //!cpp:function::
        void setInterpolation(Interpolation interp);
    
    private:
        FileTransform();
        FileTransform(const FileTransform &);
        virtual ~FileTransform();
        
        FileTransform& operator= (const FileTransform &);
        
        static void deleter(FileTransform* t);
        
        class Impl;
        friend class Impl;
        Impl * m_impl;
        Impl * getImpl() { return m_impl; }
        const Impl * getImpl() const { return m_impl; }
    };
    
    //!cpp:function::
    extern OCIOEXPORT std::ostream& operator<< (std::ostream&, const FileTransform&);
    
    
    //!rst:: //////////////////////////////////////////////////////////////////
    
    //!cpp:class::
    class OCIOEXPORT GroupTransform : public Transform
    {
    public:
        //!cpp:function::
        static GroupTransformRcPtr Create();
        
        //!cpp:function::
        virtual TransformRcPtr createEditableCopy() const;
        
        //!cpp:function::
        virtual TransformDirection getDirection() const;
        //!cpp:function::
        virtual void setDirection(TransformDirection dir);
        
        //!cpp:function::
        ConstTransformRcPtr getTransform(int index) const;
        //!cpp:function:This returns the actual internal transform, not a copy.:
        TransformRcPtr getEditableTransform(int index);
        
        //!cpp:function::
        int size() const;
        //!cpp:function::
        void push_back(const ConstTransformRcPtr& transform);
        //!cpp:function::
        void clear();
        //!cpp:function::
        bool empty() const;
    
    private:
        GroupTransform();
        GroupTransform(const GroupTransform &);
        virtual ~GroupTransform();
        
        GroupTransform& operator= (const GroupTransform &);
        
        static void deleter(GroupTransform* t);
        
        class Impl;
        friend class Impl;
        Impl * m_impl;
        Impl * getImpl() { return m_impl; }
        const Impl * getImpl() const { return m_impl; }
    };
    
    //!cpp:function::
    extern OCIOEXPORT std::ostream& operator<< (std::ostream&, const GroupTransform&);
    
    
    //!rst:: //////////////////////////////////////////////////////////////////
    
    //!cpp:class:: Represents log transform: log(color, base)
    // 
    // * The input will be clamped for negative numbers.
    // * Default base is 2.0
    // * Only the rgb channels are affected
    class OCIOEXPORT LogTransform : public Transform
    {
    public:
        //!cpp:function::
        static LogTransformRcPtr Create();
        
        //!cpp:function::
        virtual TransformRcPtr createEditableCopy() const;
        
        //!cpp:function::
        virtual TransformDirection getDirection() const;
        //!cpp:function::
        virtual void setDirection(TransformDirection dir);
        
        //!cpp:function::
        void setBase(float val);
        //!cpp:function::
        float getBase() const;
    
    private:
        LogTransform();
        LogTransform(const LogTransform &);
        virtual ~LogTransform();
        
        LogTransform& operator= (const LogTransform &);
        
        static void deleter(LogTransform* t);
        
        class Impl;
        friend class Impl;
        Impl * m_impl;
        Impl * getImpl() { return m_impl; }
        const Impl * getImpl() const { return m_impl; }
    };
    
    //!cpp:function::
    extern OCIOEXPORT std::ostream& operator<< (std::ostream&, const LogTransform&);
    
    
    //!rst:: //////////////////////////////////////////////////////////////////
    
    //!cpp:class:: Represents an MX+B Matrix transform
    class OCIOEXPORT MatrixTransform : public Transform
    {
    public:
        //!cpp:function::
        static MatrixTransformRcPtr Create();
        
        //!cpp:function::
        virtual TransformRcPtr createEditableCopy() const;
        
        //!cpp:function::
        virtual TransformDirection getDirection() const;
        //!cpp:function::
        virtual void setDirection(TransformDirection dir);
        
        //!cpp:function::
        bool equals(const MatrixTransform & other) const;
        
        //!cpp:function::
        void setValue(const float * m44, const float * offset4);
        //!cpp:function::
        void getValue(float * m44, float * offset4) const;
        
        //!rst:: **Convenience functions**
        //
        // to get the mtx and offset corresponding to higher-level concepts
        // 
        // .. note::
        //    These can throw an exception if for any component
        //    ``oldmin == oldmax. (divide by 0)``
        
        //!cpp:function::
        static void Fit(float * m44, float * offset4,
                        const float * oldmin4, const float * oldmax4,
                        const float * newmin4, const float * newmax4);
        
        //!cpp:function::
        static void Identity(float * m44, float * offset4);
        
        //!cpp:function::
        static void Sat(float * m44, float * offset4,
                        float sat, const float * lumaCoef3);
        
        //!cpp:function::
        static void Scale(float * m44, float * offset4,
                          const float * scale4);
        
        //!cpp:function::
        static void View(float * m44, float * offset4,
                         int * channelHot4,
                         const float * lumaCoef3);
    
    private:
        MatrixTransform();
        MatrixTransform(const MatrixTransform &);
        virtual ~MatrixTransform();
        
        MatrixTransform& operator= (const MatrixTransform &);
        
        static void deleter(MatrixTransform* t);
        
        class Impl;
        friend class Impl;
        Impl * m_impl;
        Impl * getImpl() { return m_impl; }
        const Impl * getImpl() const { return m_impl; }
    };
    
    //!cpp:function::
    extern OCIOEXPORT std::ostream& operator<< (std::ostream&, const MatrixTransform&);
    
}
OCIO_NAMESPACE_EXIT

#endif
