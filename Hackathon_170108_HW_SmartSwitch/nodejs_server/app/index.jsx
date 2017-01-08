import {Provider} from 'react-redux'
import React, {Component,PropTypes} from 'react'
import {Router, Route, IndexRoute,Redirect ,browserHistory} from 'react-router'
import {render} from 'react-dom'
import Nav from  './navBar/nav.jsx'
import Home from './component/home.jsx'
import Myform from './component/justForVIP.jsx'
import Exchange from './component/cExchange.jsx'
import {store} from "./store/store"


render(
    <Provider store={store}>
        <Router history={browserHistory}>
            <Route path="/" component={Nav}>
                <IndexRoute component={Home}/>
                <Route path="bImages"/>
                <Route path="lNews"/>
                <Route path="aSearches"/>
                <Route path="cExchange" component={Exchange}/>
                <Route path="vip" component={Myform}/>
                <Redirect from='*' to='/'/>
            </Route>
        </Router>
    </Provider>,
    document.getElementById('app')
)
;
