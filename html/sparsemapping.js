var sparsemapping =
[
    [ "Creation of sparse maps for robot localization", "sparsemapping.html#autotoc_md308", [
      [ "What is a map", "sparsemapping.html#autotoc_md309", null ],
      [ "Map files", "sparsemapping.html#autotoc_md310", null ],
      [ "ROS node", "sparsemapping.html#autotoc_md311", [
        [ "Inputs", "sparsemapping.html#autotoc_md312", null ],
        [ "Outputs", "sparsemapping.html#autotoc_md313", null ]
      ] ],
      [ "Tools and procedures", "sparsemapping.html#autotoc_md314", [
        [ "Record a bag", "sparsemapping.html#autotoc_md315", null ],
        [ "Filter the bag", "sparsemapping.html#autotoc_md316", null ],
        [ "Copy the bag from the robot:", "sparsemapping.html#autotoc_md317", null ],
        [ "Merging bags", "sparsemapping.html#autotoc_md318", null ],
        [ "Extracting images", "sparsemapping.html#autotoc_md319", null ],
        [ "Building a map", "sparsemapping.html#autotoc_md320", null ],
        [ "Visualization", "sparsemapping.html#autotoc_md321", null ],
        [ "Localize a single frame", "sparsemapping.html#autotoc_md322", null ],
        [ "Testing localization using two maps", "sparsemapping.html#autotoc_md323", null ],
        [ "Testing localization using a bag", "sparsemapping.html#autotoc_md324", null ],
        [ "Extract sub-maps", "sparsemapping.html#autotoc_md325", [
          [ "Merge maps", "sparsemapping.html#autotoc_md326", null ],
          [ "How to build a map efficiently", "sparsemapping.html#autotoc_md327", null ],
          [ "Map strategy for the space station", "sparsemapping.html#autotoc_md328", null ],
          [ "Growing a map when more images are acquired", "sparsemapping.html#autotoc_md329", null ],
          [ "Reducing the number of images in a map", "sparsemapping.html#autotoc_md330", null ]
        ] ]
      ] ]
    ] ],
    [ "Map building", "map_building.html", [
      [ "Detailed explanation", "map_building.html#autotoc_md268", [
        [ "Summary", "map_building.html#autotoc_md267", null ],
        [ "Reduce the number of images", "map_building.html#autotoc_md269", null ],
        [ "Setup the environment", "map_building.html#autotoc_md270", null ],
        [ "Building a map", "map_building.html#autotoc_md271", [
          [ "Map building pipeline", "map_building.html#autotoc_md272", [
            [ "Detect interest points", "map_building.html#autotoc_md273", null ],
            [ "Match images", "map_building.html#autotoc_md274", null ],
            [ "Build tracks", "map_building.html#autotoc_md275", null ],
            [ "Incremental bundle adjustment", "map_building.html#autotoc_md276", null ],
            [ "Bundle adjustment", "map_building.html#autotoc_md277", null ],
            [ "Map rebuilding", "map_building.html#autotoc_md278", null ],
            [ "Vocabulary database", "map_building.html#autotoc_md279", null ]
          ] ],
          [ "Building a SURF map only", "map_building.html#autotoc_md280", null ],
          [ "Additional options", "map_building.html#autotoc_md281", null ]
        ] ],
        [ "Map registration", "map_building.html#autotoc_md282", [
          [ "Registration in the granite lab", "map_building.html#autotoc_md283", null ],
          [ "Registration on the ISS", "map_building.html#autotoc_md284", null ],
          [ "Registration in the MGTF", "map_building.html#autotoc_md285", null ]
        ] ],
        [ "Map verification", "map_building.html#autotoc_md286", null ],
        [ "Sparse map performance and quality evaluation on the robot", "map_building.html#autotoc_md287", [
          [ "Stage the new map", "map_building.html#autotoc_md288", [
            [ "Copy the new map on the robot MLP (preferably in /data):", "map_building.html#autotoc_md289", null ],
            [ "On the MLP, move the current map aside:", "map_building.html#autotoc_md290", null ]
          ] ],
          [ "Stage the bag with images:", "map_building.html#autotoc_md292", null ],
          [ "Stage the feature counter utility (should be added to the install at one point):", "map_building.html#autotoc_md293", null ],
          [ "Launch the localization node on LLP", "map_building.html#autotoc_md294", null ],
          [ "Enable localization and the mapped landmark production (on MLP)", "map_building.html#autotoc_md295", null ],
          [ "Play the bags (on MLP)", "map_building.html#autotoc_md296", null ],
          [ "Examine the performance and features on MLP", "map_building.html#autotoc_md297", [
            [ "Look at the load with htop", "map_building.html#autotoc_md298", null ],
            [ "Watch the frequency of feature production", "map_building.html#autotoc_md299", null ],
            [ "Watch the number of features being produced:", "map_building.html#autotoc_md300", null ]
          ] ]
        ] ],
        [ "Verify localization against a sparse map on a local machine", "map_building.html#autotoc_md301", null ],
        [ "Evaluating the map without running the localization node", "map_building.html#autotoc_md302", null ]
      ] ]
    ] ],
    [ "Total Station", "total_station.html", [
      [ "Doing a Survey with the Total Station", "total_station.html#autotoc_md331", null ]
    ] ],
    [ "Granite Lab Registration", "granite_lab_registration.html", [
      [ "Locations of the control points in the granite lab used for registration", "granite_lab_registration.html#autotoc_md303", [
        [ "Point 1", "granite_lab_registration.html#autotoc_md304", null ],
        [ "Point 2", "granite_lab_registration.html#autotoc_md305", null ],
        [ "Point 3", "granite_lab_registration.html#autotoc_md306", null ],
        [ "Point 4", "granite_lab_registration.html#autotoc_md307", null ]
      ] ]
    ] ],
    [ "Using Faro", "using_faro.html", null ]
];