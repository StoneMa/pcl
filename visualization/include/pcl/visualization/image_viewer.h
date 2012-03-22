/*
 * Software License Agreement (BSD License)
 *
 *  Point Cloud Library (PCL) - www.pointclouds.org
 *  Copyright (c) 2010-2012, Willow Garage, Inc.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef PCL_VISUALIZATION_IMAGE_VISUALIZER_H__
#define	PCL_VISUALIZATION_IMAGE_VISUALIZER_H__

#include <boost/shared_array.hpp>
#include <pcl/pcl_macros.h>
#include <pcl/console/print.h>
#include <boost/signals2.hpp>
#include <pcl/visualization/interactor_style.h>
#include <pcl/visualization/vtk.h>

namespace pcl
{
  namespace visualization
  {
    typedef Eigen::Array<unsigned char, 3, 1> Vector3ub;
    static const Vector3ub green_color (0, 255, 0);
    static const Vector3ub red_color (255, 0, 0);
    static const Vector3ub blue_color (0, 0, 255);

    /** \brief ImageViewer is a class for 2D image visualization.
      *
      * Features include:
      *  - add and remove different layers with different opacity (transparency) values
      *  - add 2D geometric shapes (circles, boxes, etc) in separate layers
      *  - display RGB, monochrome, float, angle images
      *
      * Simple usage example:
      * \code
      * pcl::visualization::ImageViewer iv;
      * iv.addCircle (10, 10, 5, 1.0, 0.0, 0.0, "circles", 1.0);    // add a red, fully opaque circle with radius 5 pixels at (10,10) in layer "circles"
      * iv.addBox (10, 20, 10, 20, 0.0, 1.0, 0.0, "boxes", 0.5);    // add a green, 50% transparent box at (10,10->20,20) in layer "boxes"
      * iv.removeLayer ("circles");                                 // remove layer "circles"
      * \endcode
      * 
      * \author Radu B. Rusu, Suat Gedikli
      */
    class PCL_EXPORTS ImageViewer
    {
      public:
        /** \brief Constructor.
          * \param[in] window_title the title of the window
          */
        ImageViewer (const std::string& window_title = "");

        /** \brief Destructor. */
        virtual ~ImageViewer ();
       
        /** \brief Show a monochrome 2D image on screen.
          * \param[in] data the input data representing the image
          * \param[in] width the width of the image
          * \param[in] height the height of the image
          * \param[in] layer_id the name of the layer (default: "image")
          * \param[in] opacity the opacity of the layer (default: 1.0)
          */
        void 
        showMonoImage (const unsigned char* data, unsigned width, unsigned height,
                       const std::string &layer_id = "image", double opacity = 1.0);

        /** \brief Show a 2D RGB image on screen.
          * \param[in] data the input data representing the image
          * \param[in] width the width of the image
          * \param[in] height the height of the image
          * \param[in] layer_id the name of the layer (default: "image")
          * \param[in] opacity the opacity of the layer (default: 1.0)
          */
        void 
        showRGBImage (const unsigned char* data, unsigned width, unsigned height, 
                      const std::string &layer_id = "image", double opacity = 1.0);

        /** \brief Show a 2D image on screen, obtained from the RGB channel of a point cloud.
          * \param[in] data the input data representing the RGB point cloud 
          * \param[in] layer_id the name of the layer (default: "image")
          * \param[in] opacity the opacity of the layer (default: 1.0)
          */
        template <typename T> inline void 
        showRGBImage (const typename pcl::PointCloud<T>::ConstPtr &cloud,
                      const std::string &layer_id = "image", double opacity = 1.0)
        {
          return (showRGBImage<T> (*cloud, layer_id, opacity));
        }

        /** \brief Show a 2D image on screen, obtained from the RGB channel of a point cloud.
          * \param[in] data the input data representing the RGB point cloud 
          * \param[in] layer_id the name of the layer (default: "image")
          * \param[in] opacity the opacity of the layer (default: 1.0)
          */
        template <typename T> void 
        showRGBImage (const pcl::PointCloud<T> &cloud,
                      const std::string &layer_id = "image", double opacity = 1.0);

        /** \brief Show a 2D image (float) on screen.
          * \param[in] data the input data representing the image in float format
          * \param[in] width the width of the image
          * \param[in] height the height of the image
          * \param[in] min_value filter all values in the image to be larger than this minimum value
          * \param[in] max_value filter all values in the image to be smaller than this maximum value
          * \param[in] grayscale show data as grayscale (true) or not (false). Default: false
          * \param[in] layer_id the name of the layer (default: "image")
          * \param[in] opacity the opacity of the layer (default: 1.0)
          */
        void 
        showFloatImage (const float* data, unsigned int width, unsigned int height, 
                        float min_value = std::numeric_limits<float>::min (), 
                        float max_value = std::numeric_limits<float>::max (), bool grayscale = false,
                        const std::string &layer_id = "image", double opacity = 1.0);
        
        /** \brief Show a 2D image (unsigned short) on screen.
          * \param[in] short_image the input data representing the image in unsigned short format
          * \param[in] width the width of the image
          * \param[in] height the height of the image
          * \param[in] min_value filter all values in the image to be larger than this minimum value
          * \param[in] max_value filter all values in the image to be smaller than this maximum value
          * \param[in] grayscale show data as grayscale (true) or not (false). Default: false
          * \param[in] layer_id the name of the layer (default: "image")
          * \param[in] opacity the opacity of the layer (default: 1.0)
          */
        void
        showShortImage (const unsigned short* short_image, unsigned int width, unsigned int height, 
                        unsigned short min_value = std::numeric_limits<unsigned short>::min (), 
                        unsigned short max_value = std::numeric_limits<unsigned short>::max (), bool grayscale = false,
                        const std::string &layer_id = "image", double opacity = 1.0);

        /** \brief Show a 2D image on screen representing angle data.
          * \param[in] data the input data representing the image
          * \param[in] width the width of the image
          * \param[in] height the height of the image
          * \param[in] layer_id the name of the layer (default: "image")
          * \param[in] opacity the opacity of the layer (default: 1.0)
          */
        void 
        showAngleImage (const float* data, unsigned width, unsigned height,
                        const std::string &layer_id = "image", double opacity = 1.0);

        /** \brief Show a 2D image on screen representing half angle data.
          * \param[in] data the input data representing the image
          * \param[in] width the width of the image
          * \param[in] height the height of the image
          * \param[in] layer_id the name of the layer (default: "image")
          * \param[in] opacity the opacity of the layer (default: 1.0)
          */
        void 
        showHalfAngleImage (const float* data, unsigned width, unsigned height,
                            const std::string &layer_id = "image", double opacity = 1.0);

        /** \brief Sets the pixel at coordinates(u,v) to color while setting the neighborhood to another
          * \param[in] u the u/x coordinate of the pixel
          * \param[in] v the v/y coordinate of the pixel
          * \param[in] fg_color the pixel color
          * \param[in] bg_color the neighborhood color
          * \param[in] radius the circle radius around the pixel
          * \param[in] layer_id the name of the layer (default: "points")
          * \param[in] opacity the opacity of the layer (default: 1.0)
          */
        void
        markPoint (size_t u, size_t v, Vector3ub fg_color, Vector3ub bg_color = red_color, double radius = 3.0,
                   const std::string &layer_id = "points", double opacity = 1.0);

        /** \brief Set the window title name
          * \param[in] name the window title
          */
        void
        setWindowTitle (const std::string& name)
        {
          image_viewer_->GetRenderWindow ()->SetWindowName (name.c_str ());
        }

        /** \brief Spin method. Calls the interactor and runs an internal loop. */
        void 
        spin ();
        
        /** \brief Spin once method. Calls the interactor and updates the screen once. 
          * \param[in] time - How long (in ms) should the visualization loop be allowed to run.
          * \param[in] force_redraw - if false it might return without doing anything if the 
          * interactor's framerate does not require a redraw yet.
          */
        void 
        spinOnce (int time = 1, bool force_redraw = false);
        
        /** \brief Register a callback function for keyboard events
          * \param[in] callback  the function that will be registered as a callback for a keyboard event
          * \param[in] cookie    user data that is passed to the callback
          * \return a connection object that allows to disconnect the callback function.
          */
        boost::signals2::connection 
        registerKeyboardCallback (void (*callback) (const pcl::visualization::KeyboardEvent&, void*), 
                                  void* cookie = NULL)
        {
          return (registerKeyboardCallback (boost::bind (callback, _1, cookie)));
        }
        
        /** \brief Register a callback function for keyboard events
          * \param[in] callback  the member function that will be registered as a callback for a keyboard event
          * \param[in] instance  instance to the class that implements the callback function
          * \param[in] cookie    user data that is passed to the callback
          * \return a connection object that allows to disconnect the callback function.
          */
        template<typename T> boost::signals2::connection 
        registerKeyboardCallback (void (T::*callback) (const pcl::visualization::KeyboardEvent&, void*), 
                                  T& instance, void* cookie = NULL)
        {
          return (registerKeyboardCallback (boost::bind (callback,  boost::ref (instance), _1, cookie)));
        }
        
        /** \brief Register a callback boost::function for keyboard events
          * \param[in] cb the boost function that will be registered as a callback for a keyboard event
          * \return a connection object that allows to disconnect the callback function.
          */
        boost::signals2::connection 
        registerKeyboardCallback (boost::function<void (const pcl::visualization::KeyboardEvent&)> cb);

        /** \brief Register a callback boost::function for mouse events
          * \param[in] callback  the function that will be registered as a callback for a mouse event
          * \param[in] cookie    user data that is passed to the callback
          * \return a connection object that allows to disconnect the callback function.
          */
        boost::signals2::connection 
        registerMouseCallback (void (*callback) (const pcl::visualization::MouseEvent&, void*), 
                               void* cookie = NULL)
        {
          return (registerMouseCallback (boost::bind (callback, _1, cookie)));
        }
        
        /** \brief Register a callback function for mouse events
          * \param[in] callback  the member function that will be registered as a callback for a mouse event
          * \param[in] instance  instance to the class that implements the callback function
          * \param[in] cookie    user data that is passed to the callback
          * \return a connection object that allows to disconnect the callback function.
          */
        template<typename T> boost::signals2::connection 
        registerMouseCallback (void (T::*callback) (const pcl::visualization::MouseEvent&, void*), 
                               T& instance, void* cookie = NULL)
        {
          return (registerMouseCallback (boost::bind (callback, boost::ref (instance), _1, cookie)));
        }

        /** \brief Register a callback function for mouse events
          * \param[in] cb the boost function that will be registered as a callback for a mouse event
          * \return a connection object that allows to disconnect the callback function.
          */        
        boost::signals2::connection 
        registerMouseCallback (boost::function<void (const pcl::visualization::MouseEvent&)> cb);
        
        /** \brief Set the position in screen coordinates.
          * \param[in] x where to move the window to (X)
          * \param[in] y where to move the window to (Y)
          */
        void
        setPosition (int x, int y)
        {
          image_viewer_->SetPosition (x, y);
        }

        /** \brief Set the window size in screen coordinates.
          * \param[in] xw window size in horizontal (pixels)
          * \param[in] yw window size in vertical (pixels)
          */
        void
        setSize (int xw, int yw)
        {
          image_viewer_->SetSize (xw, yw);
        }

        /** \brief Returns true when the user tried to close the window */
        bool
        wasStopped () const { if (image_viewer_) return (stopped_); else return (true); }

        /** \brief Add a circle shape from a point and a radius
          * \param[in] x the x coordinate of the circle center
          * \param[in] y the y coordinate of the circle center
          * \param[in] radius the radius of the circle
          * \param[in] layer_id the 2D layer ID where we want the extra information to be drawn. 
          * \param[in] opacity the opacity of the layer: 0 for invisible, 1 for opaque. (default: 0.5)
          */
        bool
        addCircle (unsigned int x, unsigned int y, double radius, 
                   const std::string &layer_id = "circles", double opacity = 0.5);

        /** \brief Add a circle shape from a point and a radius
          * \param[in] x the x coordinate of the circle center
          * \param[in] y the y coordinate of the circle center
          * \param[in] radius the radius of the circle
          * \param[in] r the red channel of the color that the sphere should be rendered with (0.0 -> 1.0)
          * \param[in] g the green channel of the color that the sphere should be rendered with (0.0 -> 1.0)
          * \param[in] b the blue channel of the color that the sphere should be rendered with (0.0 -> 1.0)
          * \param[in] layer_id the 2D layer ID where we want the extra information to be drawn. 
          * \param[in] opacity the opacity of the layer: 0 for invisible, 1 for opaque. (default: 0.5)
          */
        bool
        addCircle (unsigned int x, unsigned int y, double radius, 
                   double r, double g, double b,
                   const std::string &layer_id = "circles", double opacity = 0.5);

        /** \brief Add a box and fill it in with a given color
          * \param[in] x_min the X min coordinate
          * \param[in] x_max the X max coordinate
          * \param[in] y_min the Y min coordinate
          * \param[in] y_max the Y max coordinate 
          * \param[in] layer_id the 2D layer ID where we want the extra information to be drawn. 
          * \param[in] opacity the opacity of the layer: 0 for invisible, 1 for opaque. (default: 0.5)
          */
        bool
        addBox (unsigned int x_min, unsigned int x_max, unsigned int y_min, unsigned int y_max,  
               const std::string &layer_id = "boxes", double opacity = 0.5);

        /** \brief Add a box and fill it in with a given color
          * \param[in] x_min the X min coordinate
          * \param[in] x_max the X max coordinate
          * \param[in] y_min the Y min coordinate
          * \param[in] y_max the Y max coordinate 
          * \param[in] r the red channel of the color that the sphere should be rendered with (0.0 -> 1.0)
          * \param[in] g the green channel of the color that the sphere should be rendered with (0.0 -> 1.0)
          * \param[in] b the blue channel of the color that the sphere should be rendered with (0.0 -> 1.0)
          * \param[in] layer_id the 2D layer ID where we want the extra information to be drawn. 
          * \param[in] opacity the opacity of the layer: 0 for invisible, 1 for opaque. (default: 0.5)
          */
        bool
        addBox (unsigned int x_min, unsigned int x_max, unsigned int y_min, unsigned int y_max,  
                double r, double g, double b,
                const std::string &layer_id = "boxes", double opacity = 0.5);

        /** \brief Add a new 2D rendering layer to the viewer. 
          * \param[in] layer_id the name of the layer
          * \param[in] width the width of the layer
          * \param[in] height the height of the layer
          * \param[in] opacity the opacity of the layer: 0 for invisible, 1 for opaque. (default: 0.5)
          */
        bool
        addLayer (const std::string &layer_id, int width, int height, double opacity = 0.5);

        /** \brief Remove a 2D layer given by its ID.
          * \param[in] layer_id the name of the layer
          */
        void
        removeLayer (const std::string &layer_id);
      protected:
        /** \brief Set the stopped flag back to false */
        void
        resetStoppedFlag () { if (image_viewer_) stopped_ = false; }

        /** \brief Fire up a mouse event with a specified event ID
          * \param[int] event_id the id of the event
          */
        void 
        emitMouseEvent (unsigned long event_id);
        
        /** \brief Fire up a keyboard event with a specified event ID
          * \param[int] event_id the id of the event
          */
        void 
        emitKeyboardEvent (unsigned long event_id);
        
        // Callbacks used to register for vtk command
        static void 
        MouseCallback (vtkObject*, unsigned long eid, void* clientdata, void *calldata);
        static void 
        KeyboardCallback (vtkObject*, unsigned long eid, void* clientdata, void *calldata);
        
      protected: // types
        struct ExitMainLoopTimerCallback : public vtkCommand
        {
          ExitMainLoopTimerCallback () : right_timer_id (), window () {}

          static ExitMainLoopTimerCallback* New ()
          {
            return (new ExitMainLoopTimerCallback);
          }
          virtual void 
          Execute (vtkObject* vtkNotUsed (caller), unsigned long event_id, void* call_data)
          {
            if (event_id != vtkCommand::TimerEvent)
              return;
            int timer_id = *reinterpret_cast<int*> (call_data);
            if (timer_id != right_timer_id)
              return;
            window->interactor_->TerminateApp ();
          }
          int right_timer_id;
          ImageViewer* window;
        };
        struct ExitCallback : public vtkCommand
        {
          ExitCallback () : window () {}

          static ExitCallback* New ()
          {
            return (new ExitCallback);
          }
          virtual void 
          Execute (vtkObject* caller, unsigned long event_id, void* call_data)
          {
            if (event_id != vtkCommand::ExitEvent)
              return;
            window->stopped_ = true;
            window->interactor_->TerminateApp ();
          }
          ImageViewer* window;
        };

    private:

        /** \brief Internal structure describing a layer. */
        struct Layer
        {
          Layer () : canvas (), layer_name (), opacity () {}
          vtkSmartPointer<vtkImageCanvasSource2D> canvas;
          std::string layer_name;
          double opacity;
        };

        typedef std::vector<Layer> LayerMap;

        /** \brief Add a new 2D rendering layer to the viewer. 
          * \param[in] layer_id the name of the layer
          * \param[in] width the width of the layer
          * \param[in] height the height of the layer
          * \param[in] opacity the opacity of the layer: 0 for invisible, 1 for opaque. (default: 0.5)
          * \param[in] fill_box set to true to fill in the image with one black box before starting
          */
        LayerMap::iterator
        addLayer (const std::string &layer_id, int width, int height, double opacity = 0.5, bool fill_box = true);

        boost::signals2::signal<void (const pcl::visualization::MouseEvent&)> mouse_signal_;
        boost::signals2::signal<void (const pcl::visualization::KeyboardEvent&)> keyboard_signal_;
        
        vtkSmartPointer<vtkRenderWindowInteractor> interactor_;
        vtkSmartPointer<vtkCallbackCommand> mouse_command_;
        vtkSmartPointer<vtkCallbackCommand> keyboard_command_;

        /** \brief Callback object enabling us to leave the main loop, when a timer fires. */
        vtkSmartPointer<ExitMainLoopTimerCallback> exit_main_loop_timer_callback_;
        vtkSmartPointer<ExitCallback> exit_callback_;

        /** \brief The ImageViewer widget. */
        vtkSmartPointer<vtkImageViewer> image_viewer_;
   
        /** \brief The data array representing the image. Used internally. */
        boost::shared_array<unsigned char> data_;

        /** \brief The data array (representing the image) size. Used internally. */
        size_t data_size_;

        /** \brief Set to false if the interaction loop is running. */
        bool stopped_;

        /** \brief Global timer ID. Used in destructor only. */
        int timer_id_;

        /** \brief Internal blender used to overlay 2D geometry over the image. */
        vtkSmartPointer<vtkImageBlend> blend_;
 
        /** \brief Internal list with different 2D layers shapes. */
        LayerMap layer_map_;

        struct LayerComparator
        {
          LayerComparator (const std::string &str) : str_ (str) {}
          const std::string &str_;

          bool
          operator () (const Layer &layer)
          {
            return (layer.layer_name == str_);
          }
        };

        /** \brief Set to true to reinitialize the layer connections. */
        std::vector<int> layers_to_remove_;

      public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    };
  }
}

#include <pcl/visualization/impl/image_viewer.hpp>

#endif	/* __IMAGE_VISUALIZER_H__ */

