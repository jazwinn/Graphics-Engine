#ifndef __SHAPES_HPP__
#define __SHAPES_HPP__

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>
#include <vector>
#include <array>

// Forward declarations
namespace shapes {
    struct Aabb;
    struct Frustum;
    struct Sphere;
    struct Triangle;
    struct Line;
    struct Plane;
    struct Segment;
    struct Ray;

    enum SideResult {
        eINSIDE       = -1,
        eINTERSECTING = 0,
        eOUTSIDE      = 1
    };

}

namespace shapes {

    struct Line {
        glm::vec3 start;
        glm::vec3 dir;

        Line() = default;
        Line(glm::vec3 const& _start, glm::vec3 const& _dir);
        glm::vec3 evaluate(float t) const;
    };
    static_assert(std::is_trivial<Line>());
    static_assert(std::is_standard_layout<Line>());


    struct Ray {
        glm::vec3 start;
        glm::vec3 dir;

        Ray() = default;
        Ray(glm::vec3 const& _start, glm::vec3 const& _dir);
        glm::vec3  at(float t) const;
        float intersect(Plane const& plane) const;
        float intersect(Aabb const& aabb) const;
        float intersect(Sphere const& sphere) const;
        float intersect(Triangle const& triangle) const;
    };

    static_assert(std::is_trivial<Ray>());
    static_assert(std::is_standard_layout<Ray>());


    struct Segment {
        std::array<glm::vec3, 2> points;

        Segment() = default;
        Segment(glm::vec3 const& start, glm::vec3 const& end);

        glm::vec3        closest(Segment const& rhs) const;
        float       distance(glm::vec3 const& pt) const;
        glm::vec3        at(float t) const;
        glm::vec3        dir() const;
        glm::vec3 const& operator[](int index) const;
        glm::vec3&       operator[](int index);
    };
    static_assert(std::is_trivial<Segment>());
    static_assert(std::is_standard_layout<Segment>());


    struct Plane {
        glm::vec3  normal;
        float d;

        Plane() = default;
        explicit Plane(Triangle const& triangle);
        Plane(glm::vec3 const& point, glm::vec3 const& _normal);
        Plane(glm::vec3 const& _normal, float d);

        glm::vec3 get_point() const;

        float      distance(glm::vec3 const& pt) const;
        glm::vec3       closest_point(glm::vec3 const& pt) const;
        SideResult classify(glm::vec3 const& pt) const;
        SideResult classify(Triangle const& triangle) const;
        SideResult classify(Aabb const& aabb) const;
        SideResult classify(Sphere const& sphere) const;
    };
    static_assert(sizeof(Plane) == sizeof(float) * 4);
    static_assert(std::is_trivial<Plane>());
    static_assert(std::is_standard_layout<Plane>());


    struct Triangle {
        std::array<glm::vec3, 3> points;

        Triangle() = default;
        Triangle(glm::vec3 const& a, glm::vec3 const& b, glm::vec3 const& c);
        glm::vec3 closest(glm::vec3 const& pt) const;

        void        invert();
        glm::vec3        normal() const;
        glm::vec3 const& operator[](int index) const;
        glm::vec3&       operator[](int index);
        float       area() const;
    };
    static_assert(std::is_trivial<Triangle>());
    static_assert(std::is_standard_layout<Triangle>());


    struct Sphere {
        glm::vec3  center;
        float radius;

        Sphere() = default;
        Sphere(glm::vec3 const& center, float radius);
        bool contains(const glm::vec3&  pt) const;
        bool intersects(Sphere const& rhs) const;

        static Sphere centroid(glm::vec3 const* points, size_t count);
        static Sphere centroid(glm::vec3 const* points, size_t count, glm::mat4 const& transformMatrix);
        static Sphere ritters(glm::vec3 const* points, size_t count);
        static Sphere ritters(glm::vec3 const* points, size_t count, glm::mat4 const& transformMatrix);
        static Sphere iterative(glm::vec3 const* points, size_t count, size_t iteration_count, float shrink_ratio);
        static Sphere iterative(glm::vec3 const* points, size_t count, size_t iteration_count, float shrink_ratio, glm::mat4 const& transform);
    };
    static_assert(std::is_trivial<Sphere>());
    static_assert(std::is_standard_layout<Sphere>());


    struct Aabb {
        glm::vec3 min;
        glm::vec3 max;

        Aabb() = default;
        Aabb(glm::vec3 const* points, size_t count, glm::mat4 const& transformMatrix);
        Aabb(glm::vec3 const* points, size_t count);
        Aabb(glm::vec3 const& min_point, glm::vec3 const& max_point);
        Aabb(Aabb const& bv, glm::mat4 const& _transform);
        Aabb(Aabb const& lhs, Aabb const& rhs);
        Aabb(glm::vec3 const& center, float width, float height, float depth);

        Aabb  transform(glm::mat4 const& m2w) const;
        bool  intersects(glm::vec3 const& pt) const;
        bool  intersects(Aabb const& rhs) const;
        float surface_area() const;
        float volume() const;
        glm::vec3  get_center() const;
        glm::vec3  get_extents() const;
        int   longest_axis() const;
    };
    static_assert(std::is_trivial<Aabb>());
    static_assert(std::is_standard_layout<Aabb>());


    struct Frustum {

        Frustum() = default;
        explicit Frustum(glm::mat4 const& viewProj);
        Frustum( std::array<glm::vec3, 6> const& normals,  std::array<float, 6> const& dists);

        SideResult classify(Sphere const& sphere) const;
        SideResult classify(Aabb const& aabb) const;

        /**
         * @brief
         *  Retrieves a plane on the frustum
         * @param index
         *  Index of each plane on the frustum
         * @return
         *  Returns the plane
         */
        Plane& operator[](const size_t index) {
            return planes[index];
        }

        /**
         * @brief
         *  Retrieves a plane on the frustum
         * @param index
         *  Index of each plane on the frustum
         * @return
         *  Returns the plane
         */
        Plane operator[](const size_t index) const {
            return planes[index];
        }


        std::array<Plane, 6> planes;

    };
    static_assert(std::is_trivial<Frustum>());
    static_assert(std::is_standard_layout<Frustum>());
}

#endif // __SHAPES_HPP__
