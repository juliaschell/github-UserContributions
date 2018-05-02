# github-UserContributions

Problem: 

Implement a solution to fetch the contributions of a GitHub user in the last year (365 days) using the public APIs of GitHub.

Contributions contain the issues opened by a user, the commits made on a master repository and the pull requests opened on a master repository.

The output of your library has to be in the following format: [0, 5, 3, 15, 0 ... 1, 19] where 0 is the number of contributions 365 days ago, 5 the number of contributions 364 days ago, 1 the number of contribution yesterday, 19 the number of contributions of today etc…
