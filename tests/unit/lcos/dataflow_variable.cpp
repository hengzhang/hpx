//  Copyright (c) 2010-2011 Dylan Stark
//  Copyright (c)      2011 Bryce Lelbach
//  
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>
#include <hpx/util/lightweight_test.hpp>
#include <hpx/runtime/actions/plain_action.hpp>
#include <hpx/runtime/components/plain_component_factory.hpp>
#include <hpx/runtime/naming/name.hpp>

using boost::program_options::variables_map;
using boost::program_options::options_description;

using hpx::find_here;
using hpx::get_runtime;
using hpx::init;
using hpx::finalize;

using hpx::naming::id_type;

using hpx::lcos::base_lco_with_value;
using hpx::lcos::eager_future;
using hpx::lcos::dataflow_variable;
using hpx::lcos::future_value;

using hpx::actions::plain_result_action1;
using hpx::actions::continuation;

///////////////////////////////////////////////////////////////////////////////
int print(id_type const& d_id)
{
    typedef base_lco_with_value<int>::get_value_action get_action;
    HPX_TEST_EQ(eager_future<get_action>(d_id).get(), 42);
    return 0;
}

typedef plain_result_action1<int, id_type const&, print>
    print_action;
HPX_REGISTER_PLAIN_ACTION(print_action);

///////////////////////////////////////////////////////////////////////////////
typedef dataflow_variable<int, int> dataflow_int_type;

///////////////////////////////////////////////////////////////////////////////
int hpx_main(variables_map& vm)
{
    id_type here = find_here();
    id_type there = get_runtime().get_process().next();

    std::cout << ">>> print here, there" << std::endl;
    std::cout << here << " " << there << std::endl << std::endl;

    {
        std::cout << ">>> d1 = dataflow_variable()" << std::endl;
        dataflow_int_type d1;

        std::cout << ">>> spawning { print d1 } here" << std::endl; 
        future_value<int> 
            local_spawn(eager_future<print_action>(here, d1.get_gid()));

        std::cout << ">>> spawning { print d1 } there" << std::endl; 
        future_value<int> 
            remote_spawn(eager_future<print_action>(there, d1.get_gid()));

        std::cout << ">>> bind(d1, 42)" << std::endl;
        continuation(d1.get_gid()).trigger<int>(42);

        // We have to use the eager_future here in order to make sure the
        // spawned action has completed before we let the dataflow_variable
        // go out of scope and destruct.
        local_spawn.get();
        remote_spawn.get();
    }

    // initiate shutdown of the runtime systems on all localities
    finalize();
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    // Configure application-specific options.
    options_description
        desc_commandline("usage: " HPX_APPLICATION_STRING " [options]");

    // Initialize and run HPX.
    HPX_TEST_EQ_MSG(hpx::init(desc_commandline, argc, argv), 0,
      "HPX main exited with non-zero status");
    return hpx::util::report_errors();
}

