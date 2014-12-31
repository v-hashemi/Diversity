Diversity
=========

This code is used for experiments about Preference Diversity Indices (PDI's).

The current version calculates 15 different diversity indices and uses 3 voting rules.
It generates N random profiles by three different methods (sampling from real world data, random generation (impartial culture) and random walks).
For each profile, the diversity indices are calculated alongside other properties of the profile (e.g., existence of cycle or degree of agreement between different voting methods).
All of this information is aggregated in a way that we can see the behavior of the profiles of specific diversity values for each index. For example, for a specific PDI, k% of profiles with diversity index x have a Condorcet cycle.
We ran our experiments on N = 1,000,000 profiles (which takes around 1 and a half hour).
The code was used to generate results in this paper:
http://ebooks.iospress.nl/volumearticle/36977
