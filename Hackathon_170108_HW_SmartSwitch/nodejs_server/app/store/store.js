import {createStore,applyMiddleware,bindActionCreators,compose,combineReducers} from 'redux'
import thunk from "redux-thunk"
import {reducers} from "../reducers/reducers"
export const store = createStore(combineReducers(reducers), {
    info: {
        username: "huenchao",
        age: "100",
        sex: "man"
    },
    wealth: {
        coins: 0,
        tickets: 0,
        gifts: 0
    }

}, applyMiddleware(thunk));
