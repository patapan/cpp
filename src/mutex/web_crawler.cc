/**
 * // This is the HtmlParser's API interface.
 * // You should not implement it, or speculate about its implementation
 * class HtmlParser {
 *   public:
 *     vector<string> getUrls(string url);
 * };
 * https://leetcode.com/problems/web-crawler-multithreaded/
 */
class Crawler {
private:
    std::mutex mu;
    std::condition_variable cv;
    std::unordered_set<string> seen;
    std::queue<string> q;
    std::unordered_map<std::string, std::string> cache;
    atomic<int> runningWorkers;

    std::string getHostname(std::string str) {
        if (cache.contains(str)) return cache[str];

        std::istringstream stream(str);
        std::string line;
        auto startIdx = 7; // hardcode 'http://'
        auto endIdx = str.find('/',7);
        cache[str] = str.substr(startIdx, endIdx - startIdx);
        return cache[str];
    }

    void run(HtmlParser& htmlParser) {
        while (true) {
            std::unique_lock lock(mu);

            cv.wait(lock, [this]{ return q.size() > 0 || runningWorkers.load(std::memory_order_release) == 0; });
            if (q.size() == 0 && runningWorkers.load(std::memory_order_release) == 0) break;

            runningWorkers.fetch_add(1, memory_order_acq_rel);
            
            string site = q.front();
            q.pop();
            lock.unlock();

            auto child_sites = htmlParser.getUrls(site);
            for (auto& child : child_sites) {
                if (getHostname(child) == getHostname(site) && !seen.contains(child)) {
                    lock.lock();
                    q.push(child);
                    seen.insert(child);
                    lock.unlock();
                }
            }
            runningWorkers.fetch_sub(1, memory_order_acq_rel);
            cv.notify_all();
        }

    }

public:
    vector<string> crawl(string startUrl, HtmlParser htmlParser) {
        vector<thread> workers;
        runningWorkers.store(0, std::memory_order_relaxed);
        q.push(startUrl);
        seen.insert(startUrl);
        // run
        for (int i = 0; i < 8; i++) workers.push_back(thread([this, &htmlParser]{this->run(htmlParser);}));
        // join (wait for threads to complete)
        for (int i = 0; i < 8; i++) workers[i].join();

        vector<string> ret(seen.begin(), seen.end());
        return ret;
    }
};