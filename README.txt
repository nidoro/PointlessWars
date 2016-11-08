==========================================
  Pointless Wars
==========================================
  Repository guidelines:
    This repository follows a gitflow workflow. There are two fixed branches:
      master:     stores the official release history. The name of all commits in the 
                  master branch must contain a version number tag. MUST be stable.
      develop:    serves as an integration branch for features. SHOULD be stable.
    Other types of branches are created and removed by developers throughout development:
      feature:    basically any type of change, bug fixing, addition or removal of 
                  features, etc. CAN be broken.
      rc:         a branch for release candidates.
      hotfix:     branch to fix problems encountered in master that have a quick solution.
    Branch name convention (without <>): 
      feature-<feature-name-or-description>
      rc-<version-number>
      hotfix-<issue-or-bug>
    Developers workflow: 
      Start a new feature branch from the tip of the develop branch. After the work is 
      done,  push changes into develop. When the set of features required by the next master 
      iteration is complete, create release candidate branch. If release candidate is 
      approved, push it to master. If a small problem with a quick solution is found in
      master, create a hotfix branch to create and test the fix. Then push fix to 
      master, remembering to update master version.