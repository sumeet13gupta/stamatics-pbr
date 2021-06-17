#pragma once

#include "math_definitions.h"
#include "material.h"
#include "scene.h"
#include "config.h"
#include <array>

namespace path
{
    // NOTE: in is w.r.t. rays from the camera
    struct BaseBRDF
    {
        virtual Ray sample(const Ray& in, const HitResult& hit)
        {
            // Samples a hemisphere uniformly by default
            double u1 = dist(gen);
            double u2 = dist(gen);

            double A = std::sqrt(1 - u1 * u1);
            double phi = 2 * PBR_PI * u2;

            Vec w = hit.normal;
            Vec u = normalize(cross(w, Vec { 0, 1, 0 }));
            Vec v = normalize(cross(u, w));

            return {
                hit.point,
                normalize(u * A * std::cos(phi) + v * A * std::sin(phi) + w * u1)
            };
        }

        virtual Colorf eval(const Ray& in, const HitResult& hit, const Ray& out)
        {
            // Returns hit.material.color by default
            return hit.material->color;
        }

        BaseBRDF() : dist(0.0, 1.0)
        {
            gen = std::mt19937(rd());
        }

    private:
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_real_distribution<> dist;
    };

    /** Lambertian Diffuse BRDF */
    struct DiffuseBRDF : public BaseBRDF
    {
        virtual Colorf eval(const Ray& in, const HitResult& hit, const Ray& out) override
        {
            // return normalize(out.direction);
            auto diff = cosv(out.direction, hit.normal);
            return hit.material->color * diff * 2; // 2pi for monte carlo, 1/pi for Lambertian BRDF
        }
    };

    struct SpecularBRDF : public BaseBRDF
    {
        virtual Ray sample(const Ray& in, const HitResult& hit) override
        {
            Ray refl;
            refl.direction = reflect(in.direction, hit.normal);
            refl.origin = hit.point;
            return refl;
        }

        virtual Colorf eval(const Ray& in, const HitResult& hit, const Ray& out) override
        {
            return PBR_COLOR_WHITE;
        }
    };

//oren-Nayar Diffuse BRDF
struct OrennayarBRDF : public BaseBRDF
{
    virtual Colorf eval(const Ray &in, const HitResult &hit, const Ray &out) override
        {
            double sigma = 0.6;double sq_sigma =sigma*sigma;
            double A = 1.0 - (0.5 * (sq_sigma/ (sq_sigma + 0.33)));
            double B = 0.45 * (sq_sigma / (sq_sigma + 0.09));

            double theta_in = acos(clamp(dot(out.direction*-1.0,hit.normal),-1.0,1.0));
            double theta_out = acos( clamp(dot(out.direction,hit.normal),-1.0,1.0));
            double alpha = (theta_in > theta_out) ? theta_in : theta_out;   // max
            double beta =  (theta_in > theta_out) ? theta_out : theta_in;   // min

            Vec proj_in  = in.direction + hit.normal*(cosv(in.direction*-1.0,hit.normal));
            Vec proj_out = out.direction - hit.normal*(cosv(hit.normal,out.direction));
            double cosDiff = cosv(proj_in,proj_out);       // = cos(phi_in - phi_out)
            //double phi_in =acos(dot(hit.normal, in.direction*-1.0) / std::sin(theta_in));
            //double phi_out =acos(dot(hit.normal, out.direction) / std::sin(theta_out));
            //double diff = A + B * std::cos(phi_in - phi_out) * std::sin(alpha) * std::tan(beta);
            double diff = A + B * cosDiff * std::sin(alpha) * std::tan(beta);

            return hit.material->color * diff * 2;
        }
};
}

#if 0

namespace old
{
    struct UniformSampler
    {
        const double weight = (4.0 * PBR_PI) / (PBR_NUM_SAMPLES);

        // NOTE: in is w.r.t. rays from the camera
        std::vector<Ray> operator()(const Ray &in, const HitResult &hit)
        {
            std::vector<Ray> result;
            for (int i = 0; i < PBR_NUM_SAMPLES; ++i)
            {
                double u1 = dist(gen);
                double u2 = dist(gen);

                double A = 2 * std::sqrt(u1 * (1 - u1));
                double phi = 2 * PBR_PI * u2;

                auto dir = Vec{
                    A * std::cos(phi),
                    A * std::sin(phi),
                    1 - 2 * u1};
                result.push_back(Ray{hit.point, dir});
            }

            return result;
        }

        UniformSampler() : dist(0.0, 1.0)
        {
            gen = std::mt19937(rd());
        }

    private:
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_real_distribution<> dist;
    };

    struct DiscreteSampler
    {
        const double weight = 1.0;

        // NOTE: in is w.r.t. rays from the camera
        std::vector<Ray> operator()(const Ray &in, const HitResult &hit) const
        {
            switch (hit.material.type)
            {
            case EMaterialType::DIFFUSE:
            {
                // Add a random offset between [-k/2, k/2] to each component
                double k = PBR_DISCRETE_SAMPLER_DIFFUSE_OFFSET;
                Vec d = reflect(in.direction, hit.normal);
                d.x += ((double)std::rand() / RAND_MAX) * k - (k / 2);
                d.y += ((double)std::rand() / RAND_MAX) * k - (k / 2);
                d.z += ((double)std::rand() / RAND_MAX) * k - (k / 2);

                Ray refl;
                refl.direction = normalize(d);
                refl.origin = hit.point;
                return {refl};
            }

            case EMaterialType::SPECULAR:
            {
                Ray refl;
                refl.direction = reflect(in.direction, hit.normal);
                refl.origin = hit.point;
                return {refl};
            }

            default:
                // return a ray with zero-length direction if we don't want to trace the ray further
                return {Ray{}};
            }
        }
    };

    // Create a NxNxN cubical grid inside a unit sphere centered at hit.point,
    //   where N = PBR_GRID_SAMPLER_SIZE. Join the center to each element of this cube to get
    //   the sampled rays.
    struct GridSampler
    {
        // weight = h^3 where h is the side of an element of the grid
        const double weight = std::pow(2.0 / (PBR_GRID_SAMPLER_SIZE * std::sqrt(3)), 3.0);

        // NOTE: in is w.r.t. rays from the camera
        std::vector<Ray> operator()(const Ray &in, const HitResult &hit) const
        {
            const Vec center = hit.point;
            const double half_side = 1.0 / std::sqrt(3);
            const double h = 2.0 / (PBR_GRID_SAMPLER_SIZE * std::sqrt(3));

            std::vector<Ray> result(8);
            for (short i = 0; i < 8; ++i)
            {
                // Add + or - half_side to each component
                int kx = (i == 2 || i == 3 || i == 4 || i == 7) ? -1 : 1;
                int ky = (i >= 4) ? -1 : 1;
                int kz = (i == 1 || i == 2 || i == 6 || i == 7) ? -1 : 1;

                result[i].origin = center;
                result[i].direction.x = kx * half_side;
                result[i].direction.y = ky * half_side;
                result[i].direction.z = kz * half_side;
            }

            // Three cube edges as basis
            const Vec AB = result[1].direction - result[0].direction;
            const Vec AD = result[3].direction - result[0].direction;
            const Vec AF = result[5].direction - result[0].direction;

            for (size_t i = 1; i < PBR_GRID_SAMPLER_SIZE; ++i)
            {
                for (size_t j = 1; j < PBR_GRID_SAMPLER_SIZE; ++j)
                {
                    for (size_t k = 1; k < PBR_GRID_SAMPLER_SIZE; ++k)
                    {
                        // Subdivide cube
                        Vec dir = AB * h * i + AD * h * j + AF * h * k + result[0].direction;

                        // Push ray to result
                        Ray ray{};
                        ray.direction = normalize(dir);
                        ray.origin = center;
                        result.push_back(ray);
                    }
                }
            }

            return result;
        }
    };

    // Simulates perfectly diffuse surfaces.
    struct DiffuseBRDF
    {
        Colorf operator()(const Ray &in, const HitResult &hit, const Ray &out) const
        {
            double diff = clamp(cosv(out.direction, hit.normal));
            return hit.material.color * diff;
        }
    };

    // Simulates the Phong model
    // https://en.wikipedia.org/wiki/Phong_reflection_model
    struct PhongBRDF
    {
        Colorf operator()(const Ray &in, const HitResult &hit, const Ray &out) const
        {
            const double kD = (hit.material.type == EMaterialType::SPECULAR) ? 0.05 : 0.95;
            const double kS = 1 - kD;
            const double shininess = 32;

            double diff = clamp(cosv(out.direction, hit.normal));
            double spec = std::pow(
                clamp(dot(
                    normalize(reflect(out.direction * -1, hit.normal)),
                    normalize(in.direction * -1))),
                shininess);

            return hit.material.color * (kD * diff + kS * spec);
        }
    };

    // Simulates a discrete BRDF
    struct DiscreteBRDF
    {
        // NOTE: in and out is w.r.t. rays from the camera
        Colorf operator()(const Ray &in, const HitResult &hit, const Ray &out) const
        {
            switch (hit.material.type)
            {
            case EMaterialType::DIFFUSE:
            {
                double diff = clamp(cosv(out.direction, hit.normal));
                return hit.material.color * diff;
            }

            case EMaterialType::SPECULAR:
            {
                return PBR_COLOR_WHITE;
            }

            default:
                return hit.material.color;
            }
        }
    };
}

#endif
