==========================================
 Pointless Wars
 Version: 0.0.0.0-0-closed-alpha
==========================================
Versioning guidelines:
    Given a version number TARGET.FEATURE.STEP-ASSETS-LABEL:
        The TARGET is incremented whenever the feature and assets goals of the current target are reached.
        The FEATURE is incremented whenever a new feature required by target is added or a broken feature is fixed.
        The STEP is incremented whenever a step is made towards the development or correction of a feature.
        The ASSETS is incremented whenever there is a change on the assets being used.
    
    This versioning system is used exclusively for the TARGET's branches.
    The developers are free to clone the project and work on features using whatever versioning system they want.
    When pushing the changes to the TARGET's branch, this file should be modified to mention the major changes* made and increment the version.
    Whenever the TARGET is incremented, a new branch is made for the new TARGET.

    *Are considered major changes:
        (1) Any STEP or FEATURE increment reason.
        (2) Addition or removal of source/header files from project;
        (3) Addition or removal of external dependencies from project;
    In case of 2 and 3, a building example should always be given/updated and its location should be mentioned here.
-------------------------------------------
Major changes:

Known bugs:

