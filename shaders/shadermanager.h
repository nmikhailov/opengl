#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <QGLContext>
#include <QObject>

#include <type_traits>
#include <map>
#include <memory>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/set.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/as_set.hpp>
#include <boost/fusion/include/as_map.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/push_back.hpp>

#include "shaderprogram.h"
#include "colorshader.h"

/*
 * Manages shader programs switching
 * Usage:
 * auto &shader = shader_manager->setActiveShader<MyShaderProgram>();
 * shader.setVertexBuffer(...);
 * ...
 */

class ShaderManager : public QObject {
    Q_OBJECT
public:
    typedef boost::mpl::vector<ColorShader> shader_list; // Add new Shaders here

    ShaderManager(QGLContext* context);
    virtual ~ShaderManager();

    // Load and activate shaderprogram
    template<class T>
    T& setActiveShader() {
        static_assert(std::is_base_of<ShaderProgram, T>::value &&
                !std::is_same<ShaderProgram, T>::value,
                "Wrong type, should be ShaderProgram subclass");

        ShaderProgram * &pr = boost::fusion::at_key<T>(m_programs);
        if(pr == nullptr) {
            pr = new T(m_context);
        }

        activate(pr);

        return *(dynamic_cast<T*>(pr));
    }
    
    ShaderProgram& getActiveShader();
    const ShaderProgram& getActiveShader() const;

private:
    void activate(ShaderProgram * shader);

    ShaderProgram * m_active_shader = nullptr;
    QGLContext * m_context;

private:
    typedef boost::mpl::at_c<shader_list, 0>::type default_shader;
    typedef boost::mpl::fold<
                shader_list,
                boost::mpl::vector<>,
                boost::mpl::push_back< boost::mpl::_1,
                    boost::fusion::pair< boost::mpl::_2, ShaderProgram* >
            > >::type map_types;

    boost::fusion::result_of::as_map<map_types>::type m_programs;
};

#endif // SHADERMANAGER_H
