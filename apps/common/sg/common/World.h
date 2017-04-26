// ======================================================================== //
// Copyright 2009-2017 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "sg/common/Node.h"
#include "sg/common/Serialization.h"
#include "sg/camera/Camera.h"

namespace ospray {
  namespace sg {


    struct OSPSG_INTERFACE Model : public sg::Renderable
    {
      Model();

      //InstanceGroup caches renders.  It will render children during commit, and add
         //cached rendered children during render call.  
      virtual void traverse(RenderContext &ctx, const std::string& operation) override;
      virtual void preCommit(RenderContext &ctx) override;
      virtual void postCommit(RenderContext &ctx) override;
      // virtual void preRender(RenderContext &ctx) override;
      // virtual void postRender(RenderContext &ctx) override;

      OSPModel ospModel {nullptr};
      std::vector<std::shared_ptr<Node>> nodes;
      std::shared_ptr<sg::World> oldWorld;
      OSPModel oldModel;
      int numGeometry{0};
    };

    /*! a world node */
    struct OSPSG_INTERFACE World : public sg::Renderable
    {
      World();
      virtual ~World() = default;

      /*! \brief returns a std::string with the c++ name of this class */
      virtual std::string toString() const override;

      //! serialize into given serialization state 
      virtual void serialize(sg::Serialization::State &serialization) override;

      /*! \brief return bounding box in world coordinates.

        This function can be used by the viewer(s) for calibrating
        camera motion, setting default camera position, etc. Nodes
        for which that does not apply can simpy return
        box3f(embree::empty) */
      virtual void preCommit(RenderContext &ctx) override;
      virtual void postCommit(RenderContext &ctx) override;
      virtual void preRender(RenderContext &ctx) override;
      virtual void postRender(RenderContext &ctx) override;

      OSPModel ospModel {nullptr};
      std::vector<std::shared_ptr<Node>> nodes;
      std::shared_ptr<sg::World> oldWorld;
      OSPModel oldModel;
      int numGeometry{0};
    };


    struct OSPSG_INTERFACE Instance : public sg::World
    {
      Instance();

            /*! \brief return bounding box in world coordinates.

        This function can be used by the viewer(s) for calibrating
        camera motion, setting default camera position, etc. Nodes
        for which that does not apply can simpy return
        box3f(embree::empty) */
      virtual box3f computeBounds() const override;

      //InstanceGroup caches renders.  It will render children during commit, and add
         //cached rendered children during render call.  
      virtual void traverse(RenderContext &ctx, const std::string& operation) override;
      virtual void preCommit(RenderContext &ctx) override;
      virtual void postCommit(RenderContext &ctx) override;
      virtual void preRender(RenderContext &ctx) override;
      virtual void postRender(RenderContext &ctx) override;


      OSPGeometry ospInstance {nullptr};
      //currently, nested instances do not appear to work in OSPRay.  To get around this,
      // instanced can be manually turned off for parent instancegroups.
      bool instanced {true};
      ospcommon::affine3f baseTransform;
      //    computed from baseTransform*position*rotation*scale

    protected:
      void updateInstance(RenderContext &ctx);
      void updateTransform(RenderContext &ctx);
      bool instanceDirty{true};
      ospcommon::affine3f cachedTransform;
      ospcommon::affine3f worldTransform;  
      ospcommon::affine3f oldTransform{ospcommon::one};

    };
    
  } // ::ospray::sg
} // ::ospray


