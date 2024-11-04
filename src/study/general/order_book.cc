#include <set>
#include <unordered_set>
#include <unordered_map>

/*
You can use an ordered set to fetch the orders
*/

class OrderBook {
private:
    struct Order {
        enum OrderType {
            BID,
            ASK
        };
        OrderType type;
        size_t id;
        size_t quantity;
        double price;
        enum OrderStatus {
            PENDING,
            COMPLETED,
            CANCELLED
        };
        OrderStatus status;
        time_t timestamp;
        size_t filled_volume;

    };

    std::unordered_map<int, Order> orders;

    struct OrderComparator {
        const std::unordered_map<int, Order>& orders;

        OrderComparator(const std::unordered_map<int, Order>& orders) : orders(orders) {}

        bool operator()(const int& lhs, const int& rhs) const {
            const Order& lhs_order = orders.at(lhs);
            const Order& rhs_order = orders.at(rhs);
            if (lhs_order.price == rhs_order.price) {
                return lhs_order.timestamp < rhs_order.timestamp;
            }
            return lhs_order.type == Order::OrderType::ASK ? lhs_order.price < rhs_order.price : lhs_order.price > rhs_order.price;
        }
    };


    // Instead of storing the full Order in the bids, we store only the order ID to optimize sorting.
    std::set<int, OrderComparator> bids; // sorted by largest first
    std::set<int, OrderComparator> asks; // sorted by smallest first

public:
    OrderBook() : bids(OrderComparator(orders)), asks(OrderComparator(orders)) {
        // do we need to do anything here...?
    }

    // Fill as much of the order immediately, then put the remainder in book.
    void place_order(Order& new_order){
        orders[new_order.id] = new_order;
        auto& opposite_book = new_order.type == Order::OrderType::BID ? asks : bids;

        while (opposite_book.size() > 0 && new_order.quantity > 0) {
            int book_order_id = *opposite_book.begin();
            Order& book_order = orders[book_order_id];

            if (book_order.status == Order::OrderStatus::CANCELLED) {
                opposite_book.erase(book_order.id);
                continue;
            }

            auto& bid_order = new_order.type == Order::OrderType::BID ? new_order : book_order;
            auto& ask_order = new_order.type == Order::OrderType::ASK ? new_order : book_order;

            if (bid_order.price >= ask_order.price) {
                // use the resting order fill price
                double price = bid_order.timestamp < ask_order.timestamp ? bid_order.price : ask_order.price;
                size_t quantity = std::min(new_order.quantity, book_order.quantity);
                new_order.quantity -= quantity;
                new_order.filled_volume += price * quantity;

                book_order.quantity -= quantity;
                book_order.filled_volume += price * quantity;
                
                if (book_order.quantity == 0) {
                    opposite_book.erase(book_order.id);
                    book_order.status = Order::OrderStatus::COMPLETED;
                }
            } else {
                break;
            }
        }

        if (new_order.quantity == 0) {
            // order filled
            new_order.status = Order::OrderStatus::COMPLETED;
        } else {
            auto& book = new_order.type == Order::OrderType::BID ? bids : asks;
            book.insert(new_order.id);
        }
    }

    void cancel_order(Order& order){
        orders[order.id].status = Order::OrderStatus::CANCELLED;
    }
};

int main() {}