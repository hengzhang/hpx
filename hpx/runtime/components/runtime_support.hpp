//  Copyright (c) 2007-2013 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_COMPONENTS_RUNTIME_SUPPORT_JUN_03_2008_0438PM)
#define HPX_COMPONENTS_RUNTIME_SUPPORT_JUN_03_2008_0438PM

#include <hpx/hpx_fwd.hpp>
#include <hpx/runtime/applier/applier.hpp>
#include <hpx/runtime/components/stubs/runtime_support.hpp>

namespace hpx { namespace components
{
    ///////////////////////////////////////////////////////////////////////////
    /// The \a runtime_support class is the client side representation of a
    /// \a server#runtime_support component
    class HPX_EXPORT runtime_support : public stubs::runtime_support
    {
    private:
        typedef stubs::runtime_support base_type;

    public:
        /// Create a client side representation for the existing
        /// \a server#runtime_support instance with the given global id \a gid.
        runtime_support(naming::id_type const& gid = naming::invalid_id)
          : gid_(naming::invalid_id == gid ?
                naming::id_type(
                    applier::get_applier().get_runtime_support_raw_gid(),
                    naming::id_type::unmanaged
                ) : gid)
        {}

        ///////////////////////////////////////////////////////////////////////
        // exposed functionality of this component

        /// \brief  The function \a get_factory_properties is used to
        ///         determine, whether instances of the derived component can
        ///         be created in blocks (i.e. more than one instance at once).
        ///         This function is used by the \a distributing_factory to
        ///         determine a correct allocation strategy
        int get_factory_properties(components::component_type type);
        lcos::future<int> get_factory_properties_async(components::component_type);

        /// Create a new component type using the runtime_support
        template <typename Component>
        naming::id_type create_component()
        {
            return this->base_type::template create_component<Component>(gid_);
        }

        /// Asynchronously create a new component using the runtime_support
        template <typename Component>
        lcos::future<naming::id_type> create_component_async()
        {
            return this->base_type::template create_component_async<Component>
                (gid_);
        }

#define HPX_RUNTIME_SUPPORT_CLIENT_CREATE(Z, N, D)                           \
        template <typename Component, BOOST_PP_ENUM_PARAMS(N, typename A)>   \
        lcos::future<naming::id_type>                                 \
        create_component_async(BOOST_PP_ENUM_BINARY_PARAMS(N, A, a))         \
        {                                                                    \
            return this->base_type::template create_component_async<Component>\
                (gid_, HPX_ENUM_FORWARD_ARGS(N, A, a));                      \
        }                                                                    \
                                                                             \
        template <typename Component, BOOST_PP_ENUM_PARAMS(N, typename A)>   \
        naming::id_type create_component(BOOST_PP_ENUM_BINARY_PARAMS(N, A, a))\
        {                                                                    \
            return this->base_type::template create_component<Component>     \
                (gid_, HPX_ENUM_FORWARD_ARGS(N, A, a));                      \
        }                                                                    \
    /**/

        BOOST_PP_REPEAT_FROM_TO(
            1
          , HPX_ACTION_ARGUMENT_LIMIT
          , HPX_RUNTIME_SUPPORT_CLIENT_CREATE
          , _
        )

#undef HPX_RUNTIME_SUPPORT_CLIENT_CREATE

        /// Asynchronously create N new default constructed components using
        /// the runtime_support
        std::vector<naming::id_type> bulk_create_components(
            components::component_type type, std::size_t count = 1)
        {
            return this->base_type::bulk_create_components(gid_, type, count);
        }

        /// Asynchronously create a new component using the runtime_support
        lcos::future<std::vector<naming::id_type> >
        bulk_create_components_async(components::component_type type,
            std::size_t count = 1)
        {
            return this->base_type::bulk_create_components_async(gid_, type, count);
        }

        ///////////////////////////////////////////////////////////////////////
        template <typename T, typename Config>
        naming::id_type create_memory_block(std::size_t count,
            hpx::actions::manage_object_action<T, Config> const& act)
        {
            return this->base_type::create_memory_block(gid_, count, act);
        }

        /// Asynchronously create a new memory block using the runtime_support
        template <typename T, typename Config>
        lcos::future<naming::id_type>
        create_memory_block_async(std::size_t count,
            hpx::actions::manage_object_action<T, Config> const& act)
        {
            return this->base_type::create_memory_block_async(gid_, count, act);
        }

        lcos::future<bool> load_components_async()
        {
            return this->base_type::load_components_async(gid_);
        }

        bool load_components()
        {
            return this->base_type::load_components(gid_);
        }

        lcos::future<void> call_startup_functions_async(bool pre_startup)
        {
            return this->base_type::call_startup_functions_async(gid_, pre_startup);
        }

        void call_startup_functions(bool pre_startup)
        {
            this->base_type::call_startup_functions(gid_, pre_startup);
        }

        lcos::future<void> call_shutdown_functions_async(bool pre_shutdown)
        {
            return this->base_type::call_shutdown_functions_async(gid_, pre_shutdown);
        }

        void call_shutdown_functions(bool pre_shutdown)
        {
            this->base_type::call_shutdown_functions(gid_, pre_shutdown);
        }

        /// \brief Shutdown the given runtime system
        lcos::future<void> shutdown_async(double timeout = -1)
        {
            return this->base_type::shutdown_async(gid_, timeout);
        }

        void shutdown(double timeout = -1)
        {
            this->base_type::shutdown(gid_, timeout);
        }

        /// \brief Shutdown the runtime systems of all localities
        void shutdown_all(double timeout = -1)
        {
            this->base_type::shutdown_all(gid_, timeout);
        }

        /// \brief Terminate the given runtime system
        lcos::future<void> terminate_async()
        {
            return this->base_type::terminate_async(gid_);
        }

        void terminate()
        {
            this->base_type::terminate(gid_);
        }

        /// \brief Terminate the runtime systems of all localities
        void terminate_all()
        {
            this->base_type::terminate_all(gid_);
        }

        void update_agas_cache_entry(naming::gid_type const& gid,
            naming::address const& g, boost::uint64_t count,
            boost::uint64_t offset)
        {
            this->base_type::update_agas_cache_entry(gid_, gid, g, count, offset);
        }

        /// \brief Retrieve configuration information
        void get_config(util::section& ini)
        {
            this->base_type::get_config(gid_, ini);
        }

        boost::int32_t  get_instance_count(components::component_type type)
        {
            return this->base_type::get_instance_count(gid_, type);
        }

        ///////////////////////////////////////////////////////////////////////
        naming::id_type const& get_gid() const
        {
            return gid_;
        }

    private:
        naming::id_type gid_;
    };
}}

#endif
