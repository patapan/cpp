#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

using namespace std;

string seconds_to_time(int seconds) {
    int minutes = seconds / 60;
    int hours = minutes / 60;
    stringstream ss;
    ss << setfill('0') << setw(2) << hours
       << ":"
       << setfill('0') << setw(2) << minutes
       << ":"
       << setfill('0') << setw(2) << (seconds % 60);
    return ss.str();
}

class job {
    [[nodiscard]] bool is_valid() const  {
        return job_id <= 0
            || runtime_in_seconds < 0
            || next_job_id < 0;
    }

public:
    int job_id;
    int runtime_in_seconds;
    int next_job_id;

    friend istream& operator>>(istream& is, job& job) {
        char comma1, comma2;
        is >> job.job_id
           >> comma1
           >> job.runtime_in_seconds
           >> comma2
           >> job.next_job_id;

        if (job.is_valid()) {
            is.setstate(ios::failbit);
        }

        return is;
    }

    friend ostream& operator<<(ostream& os, const job& job) {
        os << "job_id: " << job.job_id << ", "
           << "runtime_in_seconds: " << job.runtime_in_seconds << ", "
           << "next_job_id: " << job.next_job_id << endl;
        return os;
    }
};

class report {
    int _start_job;
    int _last_job;
    int _number_of_jobs;
    int _job_chain_runtime;

public:
    report(int start_job
         , int last_job
         , int number_of_jobs
         , int job_chain_runtime)
         : _start_job(start_job)
         , _last_job(last_job)
         , _number_of_jobs(number_of_jobs)
         , _job_chain_runtime(job_chain_runtime) {}

    [[nodiscard]] int average_job_time() const {
        return _job_chain_runtime / _number_of_jobs;
    }

    friend ostream& operator<<(ostream& os, report const& report) {
        os << "start_job: " << report._start_job << endl
           << "last_job: " << report._last_job << endl
           << "number_of_jobs: " << report._number_of_jobs << endl
           << "job_chain_runtime: " << seconds_to_time(report._job_chain_runtime) << endl
           << "average_job_time: " << seconds_to_time(report.average_job_time()) << endl;
        return os;
    }
};

void skip_first_line(istream& is) {
    string header;
    getline(is, header);
}

int main() {
    // skip first line - assuming there wasn't anything worth validating
    skip_first_line(cin);

    // copy input into `jobs`
    vector<job> jobs;
    istream_iterator<job> begin(cin);
    istream_iterator<job> end;
    copy(begin, end, back_inserter(jobs));

    // if we failed to parse the input print error and return a error code
    if (cin.fail() && !cin.eof()) {
        cout << "Malformed Input" << endl;
        return -1;
    }

    // find the parents of all jobs that are not the last in a chain
    unordered_map<int, job> previous_jobs;
    for_each(jobs.cbegin(), jobs.cend(), [&previous_jobs] (job const& job) {
        if (job.next_job_id != 0) {
            previous_jobs[job.next_job_id] = job;
        }
    });

    // add all job ids to a set; these will be removed later as jobs are added to chains
    unordered_set<int> unused_job_ids;
    transform(jobs.begin(), jobs.end(), inserter(unused_job_ids, unused_job_ids.end()), [] (job const& job) {
        return job.job_id;
    });

    // 1. Create a report for every chain stored in `reports`
    // 2. Remove all jobs that are a part of a chain from `unused_job_ids`
    vector<report> reports;
    for_each(jobs.cbegin(), jobs.cend(), [&] (job const & j) {
        if (j.next_job_id == 0) {

            int start_job = j.job_id;
            int last_job = j.job_id;
            int number_of_jobs = 1;
            int job_chain_runtime = j.runtime_in_seconds;

            // remove job that's a part of the chain
            unused_job_ids.erase(j.job_id);

            auto it = previous_jobs.find(j.job_id);
            while (it != previous_jobs.end()) {
                int job_id = (*it).second.job_id;
                int runtime_in_seconds = (*it).second.runtime_in_seconds;

                // update state variables
                start_job = job_id;
                job_chain_runtime += runtime_in_seconds;
                number_of_jobs += 1;

                // remove job that's a part of the chain
                unused_job_ids.erase(job_id);

                // move iterator to next job (if one exists)
                it = previous_jobs.find(job_id);
            }

            reports.emplace_back(start_job, last_job, number_of_jobs, job_chain_runtime);
        }
    });

    // if the input contained a job that was not a part of a chain consider it a failure
    for (int job_id : unused_job_ids) {
        cout << "Malformed Input" << endl;
        return -1;
    }

    // output the required report
    cout << "-" << endl;
    ostream_iterator<report> out(cout, "-\n");
    copy(reports.begin(), reports.end(), out);

    return 0;
}